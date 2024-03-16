#include <SDL.h>
#include <iostream>
#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include "Minigin.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "FPSCounterComponent.h"
#include "RotaterComponent.h"
#include "TrashTheCacheComponent.h"
#include "Command.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "CustumCommands.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// Background game object
	auto background{ std::make_shared<GameObject>() };
	background->AddComponent<ImageComponent>(
		std::make_shared<ImageComponent>(background, ResourceManager::GetInstance().LoadTexture("background.tga"))
	);
	scene.Add(background);

	// Logo game object
	auto logo{ std::make_shared<GameObject>() };
	background->AddComponent<ImageComponent>(
		std::make_shared<ImageComponent>(logo, ResourceManager::GetInstance().LoadTexture("logo.tga"))
	);
	logo->SetLocalPosition(216, 180);
	scene.Add(logo);

	// Title game object
	auto title{ std::make_shared<GameObject>() };
	auto font{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	title->AddComponent<TextComponent>(
		std::make_shared<TextComponent>(title, "Programming 4 Assignment", font)
	);
	title->SetLocalPosition(80.0f, 20.0f);
	scene.Add(title);

	// FPS counter game object
	auto fpsCounter{ std::make_shared<GameObject>() };
	fpsCounter->AddComponent<FPSCounterComponent>(
		std::make_shared<FPSCounterComponent>(fpsCounter)
	);
	fpsCounter->SetLocalPosition(0.0f, 0.0f);
	scene.Add(fpsCounter);

	// Character A game object
	auto characterA{ std::make_shared<GameObject>() };
	characterA->AddComponent<ImageComponent>(
		std::make_shared<ImageComponent>(characterA, ResourceManager::GetInstance().LoadTexture("Character.tga"))
	);
	characterA->SetLocalPosition(100.0f, 300.0f);
	scene.Add(characterA);

	// Character B game object
	auto characterB{ std::make_shared<GameObject>() };
	characterB->AddComponent<ImageComponent>(
		std::make_shared<ImageComponent>(characterB, ResourceManager::GetInstance().LoadTexture("Character.tga"))
	);
	characterB->SetLocalPosition(300.0f, 300.0f);
	scene.Add(characterB);

	// Week 4
	InputManager::GetInstance().AddInputMappingContext(characterA.get());
	InputManager::GetInstance().GetInputMappingContext(characterA.get())->AddInputAction<TestGameObjectCommand>(SDLK_f);

	InputManager::GetInstance().AddInputMappingContext(nullptr);
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<TestCommand>(SDLK_g);

}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}