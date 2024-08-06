#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>

#include "Engine.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "FPSCounterComponent.h"
#include "SpriteComponent.h"
#include "MenuComponent.h"
#include "TileManagerComponent.h"
#include "TankComponent.h"

void Load();
void CreateMainMenu();
void CreateFirstLevel();

using namespace Minigin;

void Load()
{
	CreateMainMenu();
	CreateFirstLevel();
}

void CreateMainMenu()
{
	auto scene{ SceneManager::Instance()->CreateScene("Main Menu") };	

	auto menuObject{ scene->CreateGameObject("Menu")};
	menuObject->CreateComponent<MenuComponent>(glm::ivec2{ 200, 50 }, 10);
}

void CreateFirstLevel()
{
	auto scene{ SceneManager::Instance()->CreateScene("First Level", false) };

	/*auto spriteObject{ scene->CreateGameObject() };
	auto spriteComponent{ spriteObject->CreateComponent<SpriteComponent>() };
	spriteComponent->AddSprite(ResourceManager::Instance()->LoadSprite("TestSprite1.png", 4, 1, 4), std::chrono::milliseconds{ 2000 }, "Running");
	spriteComponent->SetSprite("Running");
	spriteObject->SetLocalRotation(0);
	spriteObject->SetLocalScale(glm::vec2{ 0.25f, 0.25f });
	spriteObject->SetLocalPosition(glm::ivec2{ 0, 0 });*/

	auto fpsCounterObject{ scene->CreateGameObject("FPS Counter") };
	fpsCounterObject->CreateComponent<FPSCounterComponent>();
	fpsCounterObject->SetLocalPosition(glm::ivec2{ 60, 450 });

	auto tileManagerObject{ scene->CreateGameObject("Tile Manager")};
	tileManagerObject->CreateComponent<TileManagerComponent>(50);
	tileManagerObject->SetLocalPosition(glm::ivec2{ 0, 0 });

	auto tankObject{ scene->CreateGameObject("Player Tank") };
	tankObject->CreateComponent<TankComponent>(100.0f);
	//tankObject->SetLocalPosition(glm::ivec2{ 200, 200 });s

	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::W, InputAction::Value{}, InputAction::Trigger::Down, std::make_shared<MoveCommand>(tankObject, MoveCommand::Direction::Up));		
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::D, InputAction::Value{}, InputAction::Trigger::Down, std::make_shared<MoveCommand>(tankObject, MoveCommand::Direction::Right));				
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::S, InputAction::Value{}, InputAction::Trigger::Down, std::make_shared<MoveCommand>(tankObject, MoveCommand::Direction::Down));				
	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::A, InputAction::Value{}, InputAction::Trigger::Down, std::make_shared<MoveCommand>(tankObject, MoveCommand::Direction::Left));				
}

int main(int, char*[])
{
	Engine::Initialize("Tron");
	Engine::Run(&Load);
	Engine::Destroy();

    return 0;
}