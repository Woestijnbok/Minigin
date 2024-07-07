#include <Windows.h>
#include <SDL_keycode.h>

#include "Minigin.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "FPSCounterComponent.h"
#include "InputMappingContext.h"
#include "CustumCommands.h"

void load()
{
	auto scene{ SceneManager::GetInstance().CreateScene("Demo") };
	
	auto font{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	auto smallFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 15) };

	// Background game object
	auto background{ scene->CreateGameObject() };
	background->AddComponent<ImageComponent>(ResourceManager::GetInstance().LoadTexture("background.tga"));

	// Logo game object
	auto logo{ scene->CreateGameObject() };
	logo->AddComponent<ImageComponent>(ResourceManager::GetInstance().LoadTexture("logo.tga"));
	logo->SetLocalPosition(216, 180);

	// Title game object
	auto title{ scene->CreateGameObject() };
	title->AddComponent<TextComponent>("Programming 4 Assignment", font);
	title->SetLocalPosition(80.0f, 20.0f);

	// FPS counter game object
	auto fpsCounter{ scene->CreateGameObject() };
	fpsCounter->AddComponent<FPSCounterComponent>();
	fpsCounter->SetLocalPosition(0.0f, 0.0f);

	// music explenation object
	auto music{ scene->CreateGameObject() };
	music->AddComponent<TextComponent>("Use keys 1 and 2 to play differnt music.", smallFont, glm::vec2{ 0.0f, 80.0f });
	music->SetLocalPosition(0.0f, 0.0f);

	// sound explenation object
	auto sound{ scene->CreateGameObject() };
	sound->AddComponent<TextComponent>("Use keys 3 and 4 to play differnt sound effects.", smallFont, glm::vec2{ 0.0f, 100.0f });
	sound->SetLocalPosition(0.0f, 0.0f);

	// sound action explenation object
	auto soundAction{ scene->CreateGameObject() };
	soundAction->AddComponent<TextComponent>("Use P to stop the music and O (the letter) to stop all sounds.", smallFont, glm::vec2{ 0.0f, 120.0f });
	soundAction->SetLocalPosition(0.0f, 0.0f);

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
	Minigin::Initialize("Tron");
	Minigin::Run(load);
	Minigin::Destroy();

    return 0;
}