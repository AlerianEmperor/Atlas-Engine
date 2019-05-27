#include "App.h"

#ifndef AE_OS_ANDROID
std::string Atlas::EngineInstance::assetDirectory = "../data";
#else
std::string Atlas::EngineInstance::assetDirectory = "data";
#endif
std::string Atlas::EngineInstance::shaderDirectory = "shader";

void App::LoadContent() {

	UnlockFramerate();

#ifndef AE_OS_ANDROID
	renderTarget = new Atlas::RenderTarget(1920, 1080);
#else
	window.SetSize(GetScreenSize().x, GetScreenSize().y);
	renderTarget = new Atlas::RenderTarget(1280, 720);
	Atlas::Texture::Texture::SetAnisotropyLevel(8);
#endif

	viewport = Atlas::Viewport(0, 0, window.GetWidth(), window.GetHeight());

	auto icon = Atlas::Texture::Texture2D("icon.png");
	window.SetIcon(&icon);

	font = Atlas::Font("roboto.ttf", 88, 10);

	camera = Atlas::Camera(47.0f, 2.0f, .25f, 400.0f,
		vec3(30.0f, 25.0f, 0.0f), vec2(-3.14f / 2.0f, 0.0f));

	scene = Atlas::Scene::Scene(vec3(-2048.0f), vec3(2048.0f));

#ifdef AE_OS_ANDROID
	touchHandler = Atlas::Input::TouchHandler(&camera, 1.5f, 7.0f, 6.0f);
#else
	mouseHandler = Atlas::Input::MouseHandler(&camera, 1.5f, 6.0f);
	keyboardHandler = Atlas::Input::KeyboardHandler(&camera, 7.0f, 6.0f);
	controllerHandler = Atlas::Input::ControllerHandler(&camera, 1.5f, 7.0f, 6.0f, 5000.0f);
#endif

	Atlas::Events::EventManager::ControllerDeviceEventDelegate.Subscribe(
		[this](Atlas::Events::ControllerDeviceEvent event) {
		if (event.type == AE_CONTROLLER_ADDED) {
			useControllerHandler = true;
		}
		else if (event.type == AE_CONTROLLER_REMOVED) {
			useControllerHandler = false;
		}
	});

	Atlas::Events::EventManager::KeyboardEventDelegate.Subscribe(
		[this](Atlas::Events::KeyboardEvent event) {
		if (event.keycode == AE_KEY_ESCAPE) {
			Exit();
		}
	});

	cubeMesh = Atlas::Mesh::Mesh("cube.dae");
	sponzaMesh = Atlas::Mesh::Mesh("sponza/sponza.dae");
	treeMesh = Atlas::Mesh::Mesh("tree.dae");
	treeMesh.cullBackFaces = false;

	cubeActor = Atlas::Actor::MovableMeshActor(&cubeMesh);
	treeActor = Atlas::Actor::StaticMeshActor(&treeMesh, scale(mat4(1.0f), vec3(3.0f)));
	sponzaActor = Atlas::Actor::StaticMeshActor(&sponzaMesh, scale(mat4(1.0f), vec3(.05f)));

	directionalLight = Atlas::Lighting::DirectionalLight(AE_STATIONARY_LIGHT);
#ifdef AE_OS_ANDROID
	directionalLight.direction = vec3(0.0f, -1.0f, 0.5f);
	directionalLight.ambient = 0.005f;
#else
	directionalLight.direction = vec3(0.0f, -1.0f, 0.1f);
	directionalLight.ambient = 0.05f;
#endif
	directionalLight.color = vec3(253, 194, 109) / 255.0f;

	// Cascaded shadow mapping
	// directionalLight.AddShadow(300.0f, 0.01f, 1024, 4, 0.7f, camera);
	// Shadow mapping that is fixed to a point
	mat4 orthoProjection = glm::ortho(-100.0f, 100.0f, -70.0f, 120.0f, -120.0f, 120.0f);
#ifndef AE_OS_ANDROID
	directionalLight.AddShadow(200.0f, 0.01f, 4096, vec3(0.0f), orthoProjection);
	directionalLight.GetShadow()->sampleCount = 1;
	directionalLight.AddVolumetric(renderTarget->GetWidth() / 2, renderTarget->GetHeight() / 2, 20, -0.5f);
#else
	directionalLight.AddShadow(200.0f, 0.01f, 2048, vec3(0.0f), orthoProjection);
	directionalLight.GetShadow()->sampleCount = 8;
	directionalLight.AddVolumetric(renderTarget->GetWidth() / 2, renderTarget->GetHeight() / 2, 10, -0.5f);
#endif

	auto pointLight1 = Atlas::Lighting::PointLight(AE_STATIONARY_LIGHT);
	pointLight1.location = vec3(24.35f, 6.5f, 7.1f);
	pointLight1.color = 2.0f * vec3(255.0f, 128.0f, 0.0f) / 255.0f;
	pointLight1.AddShadow(0.0f, 512);

	auto pointLight2 = Atlas::Lighting::PointLight(AE_STATIONARY_LIGHT);
	pointLight2.location = vec3(24.35f, 6.5f, -11.0f);
	pointLight2.color = 2.0f * vec3(255.0f, 128.0f, 0.0f) / 255.0f;
	pointLight2.AddShadow(0.0f, 512);

	auto pointLight3 = Atlas::Lighting::PointLight(AE_STATIONARY_LIGHT);
	pointLight3.location = vec3(-31.0f, 6.5f, 7.1f);
	pointLight3.color = 2.0f * vec3(255.0f, 128.0f, 0.0f) / 255.0f;
	pointLight3.AddShadow(0.0f, 512);

	auto pointLight4 = Atlas::Lighting::PointLight(AE_STATIONARY_LIGHT);
	pointLight4.location = vec3(-31.0f, 6.5f, -11.0f);
	pointLight4.color = 2.0f * vec3(255.0f, 128.0f, 0.0f) / 255.0f;
	pointLight4.AddShadow(0.0f, 512);

	scene.Add(&cubeActor);
	scene.Add(&sponzaActor);
	scene.Add(&treeActor);

	scene.Add(&directionalLight);

	scene.Add(&pointLight1);
	scene.Add(&pointLight2);
	scene.Add(&pointLight3);
	scene.Add(&pointLight4);

	audioData = new Atlas::Audio::AudioData("MenuTheme2_final.wav");
	audioStream = new Atlas::Audio::AudioStream(audioData);

	Atlas::Audio::AudioManager::AddMusic(audioStream);

}

void App::UnloadContent() {



}

void App::Update(float deltaTime) {

#ifdef AE_OS_ANDROID
	touchHandler.Update(&camera, deltaTime);
#else
	if (!useControllerHandler) {
		mouseHandler.Update(&camera, deltaTime);
		keyboardHandler.Update(&camera, deltaTime);
	}
	else {
		controllerHandler.Update(&camera, deltaTime);
	}
#endif

	camera.UpdateView();
	camera.UpdateProjection();

	scene.Update(&camera, deltaTime);

}

void App::Render(float deltaTime) {

	masterRenderer.RenderScene(&viewport, renderTarget, &camera, &scene);

	float averageFramerate = Atlas::Clock::GetAverage();

	std::string out = "Average " + std::to_string(averageFramerate) + " ms  Currently " + std::to_string(deltaTime) + " ms";

	masterRenderer.textRenderer.Render(&viewport, &font, out, 0, 0, vec4(1.0f, 0.0f, 0.0f, 1.0f), 2.5f / 10.0f);

}

Atlas::EngineInstance* GetEngineInstance() {

	return new App();

}

/*
void Main::DisplayLoadingScreen() {

	float textWidth, textHeight;
	font.ComputeDimensions("Lädt...", 2.5f, &textWidth, &textHeight);

    window->Clear();

	auto screenSize = Engine::GetScreenSize();

#ifndef AE_OS_ANDROID
    screenSize.x = 1280;
	screenSize.y = 720;
#endif

	float x = screenSize.x / 2 - textWidth / 2;
	float y = screenSize.y / 2 - textHeight / 2;

	masterRenderer.textRenderer.Render(&viewport, &font, "Lädt...", x, y, vec4(1.0f, 1.0f, 1.0f, 1.0f), 2.5f);

	window->Update();

}
*/