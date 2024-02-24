#include <SDL.h>
#include <iostream>
#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Texture2D.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto background{ std::make_shared<GameObject>() };
	if (background->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(background)))
	{
		std::cout << "yaas" << std::endl;
	}
	else std::cout << "nein" << std::endl;
	if (background->AddComponent<Texture2D>(ResourceManager::GetInstance().LoadTexture(background, "background.tga")))
	{
		std::cout << "yaas" << std::endl;
	}
	else std::cout << "nein" << std::endl;

	scene.Add(background);

	scene;
	background;

	//auto go = std::make_shared<GameObject>();
	////go->SetTexture("background.tga");
	//scene.Add(go);

	//go = std::make_shared<GameObject>();
	////go->SetTexture("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	///*auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<TextObject>("Programming 4 Assignment", font);
	//to->SetPosition(80.0f, 20.0f);
	//scene.Add(to);*/
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}