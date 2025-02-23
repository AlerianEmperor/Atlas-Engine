#ifndef AE_POSTPROCESSRENDERER_H
#define AE_POSTPROCESSRENDERER_H

#include "../System.h"
#include "Renderer.h"

namespace Atlas {

	namespace Renderer {

		class PostProcessRenderer : public Renderer {

		public:
			PostProcessRenderer() = default;

            void Init(Graphics::GraphicsDevice* device);

            void Render(Viewport* viewport, RenderTarget* target, Camera* camera,
                Scene::Scene* scene, Graphics::CommandList* commandList);

		private:
            struct alignas(16) Uniforms {
                float exposure;
                float whitePoint;
                float saturation;
                float timeInMilliseconds;
                int32_t bloomPasses;
                float aberrationStrength;
                float aberrationReversed;
                float vignetteOffset;
                float vignettePower;
                float vignetteStrength;
                vec4 vignetteColor;
            };

			void SetUniforms(Camera* camera, Scene::Scene* scene);

            PipelineConfig GetMainPipelineConfig();

            PipelineConfig GetMainPipelineConfig(const Ref<Graphics::FrameBuffer> frameBuffer);

            PipelineConfig mainPipelineSwapChainConfig;
            PipelineConfig mainPipelineFrameBufferConfig;
            PipelineConfig sharpenPipelineConfig;

            Ref<Graphics::MultiBuffer> uniformBuffer;

		};

	}

}

#endif