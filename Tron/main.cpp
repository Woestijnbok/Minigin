#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <functional>

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
#include "PlayerTankComponent.h"
#include "AimCommand.h"
#include "BulletManagerComponent.h"
#include "FireCommand.h"
#include "ScoreBoardComponent.h"
#include "BlueTankComponent.h"
#include "TankManagerComponent.h"
#include "PurpleTankComponent.h"
#include "Loaders.h"
#include "MenuCommand.h"

void Load();
void CreateMainMenu();

using namespace Minigin;

void Load()
{
	CreateMainMenu();
}

void CreateMainMenu()
{
	auto scene{ SceneManager::Instance()->CreateScene("Main Menu") };	

	auto menuObject{ scene->CreateGameObject("Menu")};
	menuObject->CreateComponent<MenuComponent>(glm::ivec2{ 200, 50 }, 10);

	InputManager::Instance()->AddController();
	InputManager::Instance()->AddController();

	InputManager::Instance()->GetKeyboard().AddInputAction(Keyboard::Key::Backspace, InputAction::Trigger::Pressed, std::make_shared<MenuCommand>());
}

int main(int, char*[])
{
	Engine::Initialize("Tron");
	Engine::Run(&Load);
	Engine::Destroy();

    return 0;
}