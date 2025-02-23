#include "VolumetricRenderer.h"
#include "../lighting/DirectionalLight.h"
#include "../common/RandomHelper.h"

namespace Atlas {

    namespace Renderer {

        void VolumetricRenderer::Init(Graphics::GraphicsDevice *device) {

            this->device = device;

            const int32_t filterSize = 4;
            blurFilter.CalculateBoxFilter(filterSize);

            volumetricPipelineConfig = PipelineConfig("volumetric/volumetric.csh");

            horizontalBlurPipelineConfig = PipelineConfig("bilateralBlur.csh",
                {"HORIZONTAL", "BLUR_RGB", "DEPTH_WEIGHT"});
            verticalBlurPipelineConfig = PipelineConfig("bilateralBlur.csh",
                {"VERTICAL", "BLUR_RGB", "DEPTH_WEIGHT"});

            resolvePipelineConfig = PipelineConfig("volumetric/volumetricResolve.csh");

            volumetricUniformBuffer = Buffer::UniformBuffer(sizeof(VolumetricUniforms));
            resolveUniformBuffer = Buffer::UniformBuffer(sizeof(ResolveUniforms));
            blurWeightsUniformBuffer = Buffer::UniformBuffer(sizeof(vec4) * (size_t(filterSize / 4) + 1));

            auto samplerDesc = Graphics::SamplerDesc {
                .filter = VK_FILTER_NEAREST,
                .mode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                .compareEnabled = true
            };
            shadowSampler = device->CreateSampler(samplerDesc);

        }

        void VolumetricRenderer::Render(Viewport* viewport, RenderTarget* target,
            Camera* camera, Scene::Scene* scene, Graphics::CommandList* commandList) {

            Graphics::Profiler::BeginQuery("Render volumetric");

            auto volumetricPipeline = PipelineManager::GetPipeline(volumetricPipelineConfig);
            commandList->BindPipeline(volumetricPipeline);

            auto lowResDepthTexture = target->GetData(target->GetVolumetricResolution())->depthTexture;
            auto depthTexture = target->GetData(FULL_RES)->depthTexture;

            commandList->ImageMemoryBarrier(target->volumetricTexture.image,
                VK_IMAGE_LAYOUT_GENERAL, VK_ACCESS_SHADER_WRITE_BIT);

            commandList->BindImage(target->volumetricTexture.image, 3, 0);
            commandList->BindImage(lowResDepthTexture->image, lowResDepthTexture->sampler, 3, 1);

            auto lights = scene->GetLights();
            if (scene->sky.sun) {
                lights.push_back(scene->sky.sun.get());
            }

            ivec2 res = ivec2(target->volumetricTexture.width, target->volumetricTexture.height);

            Graphics::Profiler::BeginQuery("Ray marching");

            for (auto& light : lights) {
                const int32_t groupSize = 8;

                res = ivec2(target->GetWidth(), target->GetHeight());

                ivec2 groupCount = res / groupSize;
                groupCount.x += ((res.x % groupSize == 0) ? 0 : 1);
                groupCount.y += ((res.y % groupSize == 0) ? 0 : 1);

                auto volumetric = light->GetVolumetric();
                auto shadow = light->GetShadow();

                if (light->type != AE_DIRECTIONAL_LIGHT || !volumetric || !shadow) continue;

                auto directionalLight = (Lighting::DirectionalLight*)light;
                vec3 direction = normalize(vec3(camera->viewMatrix * vec4(directionalLight->direction, 0.0f)));

                VolumetricUniforms uniforms;
                uniforms.sampleCount = volumetric->sampleCount;
                uniforms.intensity = volumetric->intensity * light->intensity;
                uniforms.seed = Common::Random::SampleFastUniformFloat();

                uniforms.light.direction = vec4(direction, 0.0);
                uniforms.light.color = vec4(light->color, 0.0);
                uniforms.light.shadow.cascadeCount = shadow->componentCount;

                commandList->BindImage(shadow->maps.image, shadowSampler, 3, 2);

                auto& shadowUniform = uniforms.light.shadow;
                for (int32_t i = 0; i < MAX_SHADOW_CASCADE_COUNT + 1; i++) {
                    auto& cascadeUniform = shadowUniform.cascades[i];
                    auto cascadeString = "light.shadow.cascades[" + std::to_string(i) + "]";
                    if (i < shadow->componentCount) {
                        auto cascade = &shadow->components[i];
                        cascadeUniform.distance = cascade->farDistance;
                        cascadeUniform.cascadeSpace = cascade->projectionMatrix *
                            cascade->viewMatrix * camera->invViewMatrix;
                    }
                    else {
                        cascadeUniform.distance = camera->farPlane;
                    }
                }

                auto fog = scene->fog;
                bool fogEnabled = fog && fog->enable;

                uniforms.fogEnabled = fogEnabled ? 1 : 0;

                if (fogEnabled) {
                    auto& fogUniform = uniforms.fog;
                    fogUniform.color = vec4(fog->color, 1.0f);
                    fogUniform.density = fog->density;
                    fogUniform.heightFalloff = fog->heightFalloff;
                    fogUniform.height = fog->height;
                    fogUniform.scatteringAnisotropy = glm::clamp(fog->scatteringAnisotropy, -0.999f, 0.999f);
                }

                volumetricUniformBuffer.SetData(&uniforms, 0, 1);
                commandList->BindBuffer(volumetricUniformBuffer.Get(), 3, 3);

                commandList->Dispatch(groupCount.x, groupCount.y, 1);
            }

            Graphics::Profiler::EndQuery();

            std::vector<Graphics::BufferBarrier> bufferBarriers;
            std::vector<Graphics::ImageBarrier> imageBarriers;

            {
                Graphics::Profiler::BeginQuery("Bilateral blur");

                const int32_t groupSize = 256;

                std::vector<float> kernelWeights;
                std::vector<float> kernelOffsets;

                blurFilter.GetLinearized(&kernelWeights, &kernelOffsets, false);

                auto mean = (kernelWeights.size() - 1) / 2;
                kernelWeights = std::vector<float>(kernelWeights.begin() + mean, kernelWeights.end());
                kernelOffsets = std::vector<float>(kernelOffsets.begin() + mean, kernelOffsets.end());

                auto kernelSize = int32_t(kernelWeights.size() - 1);

                auto horizontalBlurPipeline = PipelineManager::GetPipeline(horizontalBlurPipelineConfig);
                auto verticalBlurPipeline = PipelineManager::GetPipeline(verticalBlurPipelineConfig);

                blurWeightsUniformBuffer.SetData(kernelWeights.data(), 0, 1);

                commandList->BindImage(lowResDepthTexture->image, lowResDepthTexture->sampler, 3, 2);
                commandList->BindBuffer(blurWeightsUniformBuffer.Get(), 3, 4);

                ivec2 groupCount = ivec2(res.x / groupSize, res.y);
                groupCount.x += ((res.x % groupSize == 0) ? 0 : 1);
                imageBarriers = {
                    {target->volumetricTexture.image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_SHADER_READ_BIT},
                    {target->swapVolumetricTexture.image, VK_IMAGE_LAYOUT_GENERAL, VK_ACCESS_SHADER_WRITE_BIT},
                };
                commandList->PipelineBarrier(imageBarriers, bufferBarriers);

                commandList->BindPipeline(horizontalBlurPipeline);
                commandList->PushConstants("constants", &kernelSize);

                commandList->BindImage(target->swapVolumetricTexture.image, 3, 0);
                commandList->BindImage(target->volumetricTexture.image, target->volumetricTexture.sampler, 3, 1);

                commandList->Dispatch(groupCount.x, groupCount.y, 1);

                groupCount = ivec2(res.x, res.y / groupSize);
                groupCount.y += ((res.y % groupSize == 0) ? 0 : 1);

                imageBarriers = {
                    {target->swapVolumetricTexture.image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_SHADER_READ_BIT},
                    {target->volumetricTexture.image, VK_IMAGE_LAYOUT_GENERAL, VK_ACCESS_SHADER_WRITE_BIT},
                };
                commandList->PipelineBarrier(imageBarriers, bufferBarriers);

                commandList->BindPipeline(verticalBlurPipeline);
                commandList->PushConstants("constants", &kernelSize);

                commandList->BindImage(target->volumetricTexture.image, 3, 0);
                commandList->BindImage(target->swapVolumetricTexture.image, target->swapVolumetricTexture.sampler, 3, 1);

                commandList->Dispatch(groupCount.x, groupCount.y, 1);

                Graphics::Profiler::EndQuery();
            }

            imageBarriers = {
                {target->volumetricTexture.image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_SHADER_READ_BIT},
                {target->lightingTexture.image, VK_IMAGE_LAYOUT_GENERAL, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT},
            };
            commandList->PipelineBarrier(imageBarriers, bufferBarriers);

            {
                Graphics::Profiler::BeginQuery("Resolve");

                const int32_t groupSize = 8;

                res = ivec2(target->GetWidth(), target->GetHeight());

                ivec2 groupCount = res / groupSize;
                groupCount.x += ((res.x % groupSize == 0) ? 0 : 1);
                groupCount.y += ((res.y % groupSize == 0) ? 0 : 1);

                auto clouds = scene->sky.clouds;
                auto cloudsEnabled = clouds && clouds->enable;

                auto fog = scene->fog;
                bool fogEnabled = fog && fog->enable;

                resolvePipelineConfig.ManageMacro("CLOUDS", cloudsEnabled);
                resolvePipelineConfig.ManageMacro("FOG", fogEnabled);

                auto resolvePipeline = PipelineManager::GetPipeline(resolvePipelineConfig);
                commandList->BindPipeline(resolvePipeline);

                commandList->BindImage(target->lightingTexture.image, 3, 0);
                commandList->BindImage(target->volumetricTexture.image, target->volumetricTexture.sampler, 3, 1);
                commandList->BindImage(lowResDepthTexture->image, lowResDepthTexture->sampler, 3, 2);
                commandList->BindImage(depthTexture->image, depthTexture->sampler, 3, 4);

                ResolveUniforms uniforms;
                uniforms.cloudsEnabled = cloudsEnabled ? 1 : 0;
                uniforms.fogEnabled = fogEnabled ? 1 : 0;
                uniforms.downsampled2x = target->GetVolumetricResolution() == RenderResolution::HALF_RES ? 1 : 0;

                if (fogEnabled) {
                    auto& fogUniform = uniforms.fog;
                    fogUniform.color = vec4(fog->color, 1.0f);
                    fogUniform.density = fog->density;
                    fogUniform.heightFalloff = fog->heightFalloff;
                    fogUniform.height = fog->height;
                    fogUniform.scatteringAnisotropy = glm::clamp(fog->scatteringAnisotropy, -0.999f, 0.999f);
                }

                if (cloudsEnabled) {
                    commandList->BindImage(target->volumetricCloudsTexture.image, target->volumetricCloudsTexture.sampler, 3, 3);
                }

                resolveUniformBuffer.SetData(&uniforms, 0, 1);
                commandList->BindBuffer(resolveUniformBuffer.Get(), 3, 5);

                commandList->Dispatch(groupCount.x, groupCount.y, 1);

                Graphics::Profiler::EndQuery();
            }

            commandList->ImageMemoryBarrier(target->lightingTexture.image,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ACCESS_SHADER_READ_BIT);

            Graphics::Profiler::EndQuery();

        }

    }

}