#include "MasterRenderer.h"
#include "helper/GeometryHelper.h"

string MasterRenderer::rectangleVertexPath = "rectangle.vsh";
string MasterRenderer::rectangleFragmentPath = "rectangle.fsh";

MasterRenderer::MasterRenderer() {

	vertexArray = GeometryHelper::GenerateRectangleVertexArray();

	rectangleShader = new Shader();

	rectangleShader->AddComponent(VERTEX_SHADER, rectangleVertexPath);
	rectangleShader->AddComponent(FRAGMENT_SHADER, rectangleFragmentPath);

	rectangleShader->Compile();

	texturedRectangleShader = new Shader();

	texturedRectangleShader->AddComponent(VERTEX_SHADER, rectangleVertexPath);
	texturedRectangleShader->AddComponent(FRAGMENT_SHADER, rectangleFragmentPath);

	texturedRectangleShader->AddMacro("TEXTURE");

	texturedRectangleShader->Compile();

	GetUniforms();

	geometryRenderer = new GeometryRenderer();
	terrainRenderer = new TerrainRenderer();
	directionalShadowRenderer = new DirectionalShadowRenderer();
	directionalVolumetricRenderer = new DirectionalVolumetricRenderer();
	directionalLightRenderer = new DirectionalLightRenderer();
	pointLightRenderer = new PointLightRenderer();
	skyboxRenderer = new SkyboxRenderer();
	atmosphereRenderer = new AtmosphereRenderer();
	postProcessRenderer = new PostProcessRenderer();
	textRenderer = new TextRenderer();

}

void MasterRenderer::RenderScene(Window* window, RenderTarget* target, Camera* camera, Scene* scene) {

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	directionalShadowRenderer->Render(window, target, camera, scene, true);

	target->geometryFramebuffer->Bind(true);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	terrainRenderer->Render(window, target, camera, scene, true);

	geometryRenderer->Render(window, target, camera, scene, true);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	vertexArray->Bind();

	directionalVolumetricRenderer->Render(window, target, camera, scene, true);

	target->lightingFramebuffer->Bind(true);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Additive blending of light volumes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	directionalLightRenderer->Render(window, target, camera, scene, true);

	pointLightRenderer->Render(window, target, camera, scene, true);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	if (scene->sky->skybox != nullptr) {
		//skyboxRenderer->Render(window, target, camera, scene, true);
	}

	atmosphereRenderer->Render(window, target, camera, scene, true);

	target->lightingFramebuffer->Unbind();

	glDisable(GL_DEPTH_TEST);

	vertexArray->Bind();

	postProcessRenderer->Render(window, target, camera, scene, true);

}

void MasterRenderer::RenderTexture(Window* window, Texture* texture, float x, float y, float width, float height,
	bool alphaBlending, Framebuffer* framebuffer) {

	vertexArray->Bind();

	texturedRectangleShader->Bind();

	glDisable(GL_CULL_FACE);

	if (framebuffer != nullptr) {
		framebuffer->Bind(true);
	}
	else {
		glViewport(0, 0, window->viewport->width, window->viewport->height);
	}

	if (alphaBlending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	float viewportWidth = (float)(framebuffer == nullptr ? window->viewport->width : framebuffer->width);
	float viewportHeight = (float)(framebuffer == nullptr ? window->viewport->height : framebuffer->height);

	texturedRectangleProjectionMatrix->SetValue(glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportHeight));
	texturedRectangleOffset->SetValue(vec2(x, y));
	texturedRectangleScale->SetValue(vec2(width, height));
	texturedRectangleTexture->SetValue(0);

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

void MasterRenderer::RenderRectangle(Window* window, vec4 color, float x, float y, float width, float height,
	bool alphaBlending, Framebuffer* framebuffer) {

	vertexArray->Bind();

	rectangleShader->Bind();

	glDisable(GL_CULL_FACE);

	if (framebuffer != nullptr) {
		framebuffer->Bind(true);
	}
	else {
		glViewport(0, 0, window->viewport->width, window->viewport->height);
	}

	if (alphaBlending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	float viewportWidth = (float)(framebuffer == nullptr ? window->viewport->width : framebuffer->width);
	float viewportHeight = (float)(framebuffer == nullptr ? window->viewport->height : framebuffer->height);

	rectangleProjectionMatrix->SetValue(glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportHeight));
	rectangleOffset->SetValue(vec2(x, y));
	rectangleScale->SetValue(vec2(width, height));
	rectangleColor->SetValue(color);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (alphaBlending) {
		glDisable(GL_BLEND);
	}

	if (framebuffer != nullptr) {
		framebuffer->Unbind();
	}

	glEnable(GL_CULL_FACE);

}

void MasterRenderer::GetUniforms() {

	rectangleProjectionMatrix = rectangleShader->GetUniform("pMatrix");
	rectangleOffset = rectangleShader->GetUniform("rectangleOffset");
	rectangleScale = rectangleShader->GetUniform("rectangleScale");
	rectangleColor = rectangleShader->GetUniform("rectangleColor");

	texturedRectangleProjectionMatrix = texturedRectangleShader->GetUniform("pMatrix");
	texturedRectangleOffset = texturedRectangleShader->GetUniform("rectangleOffset");
	texturedRectangleScale = texturedRectangleShader->GetUniform("rectangleScale");
	texturedRectangleTexture = texturedRectangleShader->GetUniform("rectangleTexture");

}

MasterRenderer::~MasterRenderer() {

	vertexArray->DeleteContent();
	delete vertexArray;

	delete rectangleShader;
	delete texturedRectangleShader;

	delete geometryRenderer;
	delete terrainRenderer;
	delete directionalShadowRenderer;
	delete directionalVolumetricRenderer;
	delete directionalLightRenderer;
	delete pointLightRenderer;
	delete skyboxRenderer;
	delete atmosphereRenderer;
	delete postProcessRenderer;

}