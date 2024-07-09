#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <SDL_keycode.h>

#include "Engine.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "FPSCounterComponent.h"
#include "InputMappingContext.h"
#include "CustumCommands.h"

using namespace Minigin;

void load()
{
	auto scene{ SceneManager::GetInstance().CreateScene("Demo") };
	
	auto font{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	auto smallFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 15) };

	// Background game object
	auto background{ scene->CreateGameObject() };
	background->CreateComponent<ImageComponent>(ResourceManager::GetInstance().LoadTexture("background.tga"));

	// Logo game object
	auto logo{ scene->CreateGameObject() };
	logo->CreateComponent<ImageComponent>(ResourceManager::GetInstance().LoadTexture("logo.tga"));
	logo->SetLocalPosition(glm::vec2{ 216.0f, 180.0f });

	// Title game object
	auto title{ scene->CreateGameObject() };
	title->CreateComponent<TextComponent>("Programming 4 Assignment", font);
	title->SetLocalPosition(glm::vec2{ 80.0f, 20.0f });

	// FPS counter game object
	auto fpsCounter{ scene->CreateGameObject() };
	fpsCounter->CreateComponent<FPSCounterComponent>();
	fpsCounter->SetLocalPosition(glm::vec2{ 0.0f, 0.0f });

	// music explenation object
	auto music{ scene->CreateGameObject() };
	music->CreateComponent<TextComponent>("Use keys 1 and 2 to play differnt music.", smallFont);
	music->SetLocalPosition(glm::vec2{ 0.0f, 80.0f });

	// sound explenation object
	auto sound{ scene->CreateGameObject() };
	sound->CreateComponent<TextComponent>("Use keys 3 and 4 to play differnt sound effects.", smallFont);
	sound->SetLocalPosition(glm::vec2{ 0.0f, 100.0f });

	// sound action explenation object
	auto soundAction{ scene->CreateGameObject() };
	soundAction->CreateComponent<TextComponent>("Use P to stop the music and O (the letter) to stop all sounds.", smallFont);
	soundAction->SetLocalPosition(glm::vec2{ 0.0f, 120.0f });

	InputManager::GetInstance().AddInputMappingContext(nullptr);
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<TestMusic1>(false, SDLK_1, InputTrigger::down);		
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<TestMusic2>(false, SDLK_2, InputTrigger::down);	
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<TestSoundEffect1>(false, SDLK_3, InputTrigger::down);	
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<TestSoundEffect2>(false, SDLK_4, InputTrigger::down);	
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<StopMusic>(false, SDLK_p, InputTrigger::down);	
	InputManager::GetInstance().GetInputMappingContext(nullptr)->AddInputAction<StopAll>(false, SDLK_o, InputTrigger::down);	
}

int main(int, char*[]) 
{
	Engine::Initialize("Tron");
	Engine::Run(load);
	Engine::Destroy();

    return 0;
}