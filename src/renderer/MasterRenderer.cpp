#include "MasterRenderer.h"
#include "helper/GeometryHelper.h"
#include "helper/HaltonSequence.h"

namespace Atlas {

	namespace Renderer {

		std::string MasterRenderer::vertexPath = "rectangle.vsh";
		std::string MasterRenderer::fragmentPath = "rectangle.fsh";

		MasterRenderer::MasterRenderer() {

			Helper::GeometryHelper::GenerateRectangleVertexArray(vertexArray);

			rectangleShader.AddStage(AE_VERTEX_STAGE, vertexPath);
			rectangleShader.AddStage(AE_FRAGMENT_STAGE, fragmentPath);

			rectangleShader.Compile();

			texture2DShader.AddStage(AE_VERTEX_STAGE, vertexPath);
			texture2DShader.AddStage(AE_FRAGMENT_STAGE, fragmentPath);

			texture2DShader.AddMacro("TEXTURE2D");

			texture2DShader.Compile();

			texture2DArrayShader.AddStage(AE_VERTEX_STAGE, vertexPath);
			texture2DArrayShader.AddStage(AE_FRAGMENT_STAGE, fragmentPath);

			texture2DArrayShader.AddMacro("TEXTURE2D_ARRAY");

			texture2DArrayShader.Compile();

			GetUniforms();

			haltonSequence = Helper::HaltonSequence::Generate(2, 3, 16);

		}

		MasterRenderer::~MasterRenderer() {

			

		}

		void MasterRenderer::RenderScene(Viewport* viewport, RenderTarget* target, Camera* camera, Scene::Scene* scene) {

			if (scene->postProcessing.taa) {
				auto jitter = 2.0f * haltonSequence[haltonIndex] - 1.0f;
				jitter.x /= (float)target->GetWidth();
				jitter.y /= (float)target->GetHeight();

				camera->Jitter(jitter);
			}			

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);

			// Clear the lights depth maps
			framebuffer.Bind();
			auto lights = scene->GetLights();

			for (auto light : lights) {

				if (!light->GetShadow())
					continue;
				if (!light->GetShadow()->update)
					continue;

				for (int32_t i = 0; i < light->GetShadow()->componentCount; i++) {
					if (light->GetShadow()->useCubemap) {
						framebuffer.AddComponentCubemap(GL_DEPTH_ATTACHMENT,
							&light->GetShadow()->cubemap, i);
					}
					else {
						framebuffer.AddComponentTextureArray(GL_DEPTH_ATTACHMENT,
							&light->GetShadow()->maps, i);
					}

					glClear(GL_DEPTH_BUFFER_BIT);
				}
			}

			shadowRenderer.Render(viewport, target, camera, scene);
			
			glEnable(GL_CULL_FACE);

			terrainShadowRenderer.Render(viewport, target, camera, scene);

			// Shadows have been updated
			for (auto light : lights) {
				if (!light->GetShadow())
					continue;
				light->GetShadow()->update = false;
			}

			target->geometryFramebuffer.Bind(true);
			target->geometryFramebuffer.SetDrawBuffers({ GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 });

			glEnable(GL_CULL_FACE);

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			terrainRenderer.Render(viewport, target, camera, scene);

			opaqueRenderer.Render(viewport, target, camera, scene);

			glEnable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			target->geometryFramebuffer.SetDrawBuffers({ GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });

			decalRenderer.Render(viewport, target, camera, scene);

			glDisable(GL_BLEND);

			vertexArray.Bind();

			directionalVolumetricRenderer.Render(viewport, target, camera, scene);

			target->lightingFramebuffer.Bind(true);
			target->lightingFramebuffer.SetDrawBuffers({ GL_COLOR_ATTACHMENT0 });

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Additive blending of light volumes
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			directionalLightRenderer.Render(viewport, target, camera, scene);

			pointLightRenderer.Render(viewport, target, camera, scene);

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			target->lightingFramebuffer.SetDrawBuffers({ GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1 });

			if (scene->sky.cubemap) {
				skyboxRenderer.Render(viewport, target, camera, scene);
			}
			else {
				atmosphereRenderer.Render(viewport, target, camera, scene);
			}

			glDepthMask(GL_TRUE);

			oceanRenderer.Render(viewport, target, camera, scene);

			glDisable(GL_DEPTH_TEST);

			if (scene->postProcessing.taa) {
				taaRenderer.Render(viewport, target, camera, scene);

				target->historyFramebuffer.Unbind();
			}
			else {
				target->lightingFramebuffer.Unbind();
			}

			vertexArray.Bind();

			postProcessRenderer.Render(viewport, target, camera, scene);

		}

		void MasterRenderer::RenderTexture(Viewport* viewport, Texture::Texture2D* texture, float x, float y, float width, float height,
			bool alphaBlending, Framebuffer* framebuffer) {

			float viewportWidth = (float)(framebuffer == nullptr ? viewport->width : framebuffer->width);
			float viewportHeight = (float)(framebuffer == nullptr ? viewport->height : framebuffer->height);

			vec4 clipArea = vec4(0.0f, 0.0f, viewportWidth, viewportHeight);
			vec4 blendArea = vec4(0.0f, 0.0f, viewportWidth, viewportHeight);

			RenderTexture(viewport, texture, x, y, width, height, clipArea, blendArea, alphaBlending, framebuffer);

		}

		void MasterRenderer::RenderTexture(Viewport* viewport, Texture::Texture2D* texture, float x, float y, float width, float height,
			vec4 clipArea, vec4 blendArea, bool alphaBlending, Framebuffer* framebuffer) {

			vertexArray.Bind();

			texture2DShader.Bind();

			glDisable(GL_CULL_FACE);

			if (framebuffer != nullptr) {
				framebuffer->Bind(true);
			}
			else {
				glViewport(0, 0, viewport->width, viewport->height);
			}

			if (alphaBlending) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			float viewportWidth = (float)(framebuffer == nullptr ? viewport->width : framebuffer->width);
			float viewportHeight = (float)(framebuffer == nullptr ? viewport->height : framebuffer->height);

			texture2DProjectionMatrix->SetValue(glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportHeight));
			texture2DOffset->SetValue(vec2(x, y));
			texture2DScale->SetValue(vec2(width, height));
			texture2DBlendArea->SetValue(blendArea);
			texture2DClipArea->SetValue(clipArea);

			texture->Bind(GL_TEXTURE0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			if (alphaBlending) {
				glDisable(GL_BLEND);
			}

			if (framebuffer != nullptr) {
				framebuffer->Unbind();
			}

			glEnable(GL_CULL_FACE);

		}

		void MasterRenderer::RenderTexture(Viewport* viewport, Texture::Texture2DArray* texture, int32_t depth, float x,
			float y, float width, float height,  bool alphaBlending, Framebuffer* framebuffer) {

			float viewportWidth = (float)(framebuffer == nullptr ? viewport->width : framebuffer->width);
			float viewportHeight = (float)(framebuffer == nullptr ? viewport->height : framebuffer->height);

			vec4 clipArea = vec4(0.0f, 0.0f, viewportWidth, viewportHeight);
			vec4 blendArea = vec4(0.0f, 0.0f, viewportWidth, viewportHeight);

			RenderTexture(viewport, texture, depth, x, y, width, height, clipArea, blendArea, alphaBlending, framebuffer);

		}

		void MasterRenderer::RenderTexture(Viewport* viewport, Texture::Texture2DArray* texture, int32_t depth, float x, float y, float width, float height,
			vec4 clipArea, vec4 blendArea, bool alphaBlending, Framebuffer* framebuffer) {

			vertexArray.Bind();

			texture2DArrayShader.Bind();

			glDisable(GL_CULL_FACE);

			if (framebuffer != nullptr) {
				framebuffer->Bind(true);
			}
			else {
				glViewport(0, 0, viewport->width, viewport->height);
			}

			if (alphaBlending) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}


			float viewportWidth = (float)(framebuffer == nullptr ? viewport->width : framebuffer->width);
			float viewportHeight = (float)(framebuffer == nullptr ? viewport->height : framebuffer->height);

			texture2DArrayProjectionMatrix->SetValue(glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportHeight));
			texture2DArrayOffset->SetValue(vec2(x, y));
			texture2DArrayScale->SetValue(vec2(width, height));
			texture2DArrayBlendArea->SetValue(blendArea);
			texture2DArrayClipArea->SetValue(clipArea);
			texture2DArrayDepth->SetValue((float)depth);

			texture->Bind(GL_TEXTURE0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			if (alphaBlending) {
				glDisable(GL_BLEND);
			}

			if (framebuffer != nullptr) {
				framebuffer->Unbind();
			}

			glEnable(GL_CULL_FACE);

		}

		void MasterRenderer::RenderRectangle(Viewport* viewport, vec4 color, float x, float y, float width, float height,
			bool alphaBlending, Framebuffer* framebuffer) {

			float viewportWidth = (float)(framebuffer == nullptr ? viewport->width : framebuffer->width);
			float viewportHeight = (float)(framebuffer == nullptr ? viewport->height : framebuffer->height);

			if (x > viewportWidth || y > viewportHeight ||
				y + height < 0 || x + width < 0) {
				return;
			}

			vec4 clipArea = vec4(0.0f, 0.0f, viewportWidth, viewportHeight);
			vec4 blendArea = vec4(0.0f, 0.0f, viewportWidth, viewportHeight);

			RenderRectangle(viewport, color, x, y, width, height, clipArea, blendArea, alphaBlending, framebuffer);

		}

		void MasterRenderer::RenderRectangle(Viewport* viewport, vec4 color, float x, float y, float width, float height,
			vec4 clipArea, vec4 blendArea, bool alphaBlending, Framebuffer* framebuffer) {

			float viewportWidth = (float)(framebuffer == nullptr ? viewport->width : framebuffer->width);
			float viewportHeight = (float)(framebuffer == nullptr ? viewport->height : framebuffer->height);

			if (x > viewportWidth || y > viewportHeight ||
				y + height < 0 || x + width < 0) {
				return;
			}

			vertexArray.Bind();

			rectangleShader.Bind();

			glDisable(GL_CULL_FACE);

			if (framebuffer != nullptr) {
				framebuffer->Bind(true);
			}
			else {
				glViewport(0, 0, viewport->width, viewport->height);
			}

			if (alphaBlending) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			rectangleProjectionMatrix->SetValue(glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportHeight));
			rectangleOffset->SetValue(vec2(x, y));
			rectangleScale->SetValue(vec2(width, height));
			rectangleColor->SetValue(color);
			rectangleBlendArea->SetValue(blendArea);
			rectangleClipArea->SetValue(clipArea);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			if (alphaBlending) {
				glDisable(GL_BLEND);
			}

			if (framebuffer != nullptr) {
				framebuffer->Unbind();
			}

			glEnable(GL_CULL_FACE);

		}

		void MasterRenderer::Update() {

			textRenderer.Update();

			haltonIndex = (haltonIndex + 1) % haltonSequence.size();

		}

		void MasterRenderer::GetUniforms() {

			rectangleProjectionMatrix = rectangleShader.GetUniform("pMatrix");
			rectangleOffset = rectangleShader.GetUniform("rectangleOffset");
			rectangleScale = rectangleShader.GetUniform("rectangleScale");
			rectangleColor = rectangleShader.GetUniform("rectangleColor");
			rectangleBlendArea = rectangleShader.GetUniform("rectangleBlendArea");
			rectangleClipArea = rectangleShader.GetUniform("rectangleClipArea");

			texture2DProjectionMatrix = texture2DShader.GetUniform("pMatrix");
			texture2DOffset = texture2DShader.GetUniform("rectangleOffset");
			texture2DScale = texture2DShader.GetUniform("rectangleScale");
			texture2DBlendArea = texture2DShader.GetUniform("rectangleBlendArea");
			texture2DClipArea = texture2DShader.GetUniform("rectangleClipArea");

			texture2DArrayProjectionMatrix = texture2DArrayShader.GetUniform("pMatrix");
			texture2DArrayOffset = texture2DArrayShader.GetUniform("rectangleOffset");
			texture2DArrayScale = texture2DArrayShader.GetUniform("rectangleScale");
			texture2DArrayBlendArea = texture2DArrayShader.GetUniform("rectangleBlendArea");
			texture2DArrayClipArea = texture2DArrayShader.GetUniform("rectangleClipArea");
			texture2DArrayDepth = texture2DArrayShader.GetUniform("textureDepth");

		}

	}

}