#include <utility>
#include <vec2.hpp>
#include <iostream>
#include <algorithm>
#include <format>
#include <string>

#include "TankManagerComponent.h"
#include "BulletComponent.h"
#include "TankComponent.h"
#include "PlayerTankComponent.h"
#include "TileManagerComponent.h"
#include "BulletManagerComponent.h"
#include "BlueTankComponent.h"
#include "PurpleTankComponent.h"

#include "Collision.h"
#include "GameObject.h"
#include "Scene.h"

bool TankManagerComponent::m_Alive{ false };

TankManagerComponent::TankManagerComponent(Minigin::GameObject* owner) :
	Component{ owner },
	m_Tanks{},
	m_TileManager{},
	m_BulletManager{},
	m_OnGameOver{},
	m_OnLevelCompleted{}
{
	m_Alive = true;
}

TankManagerComponent::~TankManagerComponent()
{
	
	for (TankComponent* tank : m_Tanks)
	{
		tank->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
		tank->SetStatus(ControllableObject::Status::Destroyed);	
	}

	m_Alive = false;	
}

void TankManagerComponent::SetManagers(TileManagerComponent* tileManager, BulletManagerComponent* bulletManager)
{
	m_TileManager = tileManager;
	m_BulletManager = bulletManager;
}

PlayerTankComponent* TankManagerComponent::CreatePlayerTank(const std::shared_ptr<Minigin::Texture>& tankTexture, const std::shared_ptr<Minigin::Texture>& barrelTexture, int lives)
{
	PlayerTankComponent* playerTank{};

	Minigin::GameObject* object{ GetOwner()->GetScene()->CreateGameObject(std::format("Tank {}", m_Tanks.size())) };
	object->SetParent(GetOwner());	
	playerTank = object->CreateComponent<PlayerTankComponent>(this, tankTexture, barrelTexture, lives);

	m_Tanks.push_back(playerTank);

	return playerTank;
}

BlueTankComponent* TankManagerComponent::CreateBlueTank(const std::shared_ptr<Minigin::Texture>& tankTexture)
{
	BlueTankComponent* blueTank{};	

	Minigin::GameObject* object{ GetOwner()->GetScene()->CreateGameObject(std::format("Tank {}", m_Tanks.size())) };	
	object->SetParent(GetOwner());
	blueTank = object->CreateComponent<BlueTankComponent>(this, tankTexture);	

	m_Tanks.push_back(blueTank);

	return blueTank;	
}

PurpleTankComponent* TankManagerComponent::CreatePurpleTank(const std::shared_ptr<Minigin::Texture>& tankTexture)
{
	PurpleTankComponent* purpleTank{};	

	Minigin::GameObject* object{ GetOwner()->GetScene()->CreateGameObject(std::format("Tank {}", m_Tanks.size())) };
	object->SetParent(GetOwner());
	purpleTank = object->CreateComponent<PurpleTankComponent>(this, tankTexture);	

	m_Tanks.push_back(purpleTank);

	return purpleTank;
}

const std::vector<TankComponent*>& TankManagerComponent::GetTanks() const
{
	return m_Tanks;
}

std::vector<PlayerTankComponent*> TankManagerComponent::GetPlayerTanks() const
{
	std::vector<PlayerTankComponent*> players{};

	for (TankComponent* tank : m_Tanks)
	{
		PlayerTankComponent* player{ dynamic_cast<PlayerTankComponent*>(tank) };
		if (player != nullptr)
		{
			players.push_back(player);	
		}
	}

	return players;
}

void TankManagerComponent::CheckCollision(BulletComponent* bullet)
{
	if (bullet->GetOwner()->GetStatus() != ControllableObject::Status::Enabled) return;

	const glm::ivec2 bulletPosition{bullet->GetOwner()->GetLocalTransform().GetPosition() };

	for (TankComponent* tank : m_Tanks)
	{
		if (bullet->GetTank() != tank)
		{
			if (Minigin::PointInsideRectangle(bulletPosition, tank->GetCollisionRectangle()))	
			{
				bullet->GetOwner()->SetStatus(ControllableObject::Status::Destroyed);
				tank->Hit();
			}
		}
	}
}

void TankManagerComponent::RemoveTank(TankComponent* tank)
{
	m_Tanks.erase(std::remove(m_Tanks.begin(), m_Tanks.end(), tank), m_Tanks.end());
}

glm::ivec2 TankManagerComponent::GetRandomPosition() const
{
	glm::ivec2 position{ m_TileManager->GetRandomPosition() };

	while (std::ranges::any_of(m_Tanks, [&position](TankComponent* tank) -> bool { return tank->GetOwner()->GetLocalTransform().GetPosition() == position; }))	
	{
		position = m_TileManager->GetRandomPosition();
	}

	return position;
}

bool TankManagerComponent::CanMove(TankComponent* tank, MoveCommand::Direction direction) const
{
	return m_TileManager->CanMove(tank, direction);
}

bool TankManagerComponent::CanMove(const glm::ivec2& position, MoveCommand::Direction direction) const
{
	return m_TileManager->CanMove(position, direction);
}

void TankManagerComponent::AddBullet(TankComponent* tank) const
{
	m_BulletManager->AddBullet(tank);
}

int TankManagerComponent::GetTileSize() const
{
	return m_TileManager->GetTileSize();	
}

Minigin::Subject<>& TankManagerComponent::OnGameOver()
{
	return m_OnGameOver;
}

Minigin::Subject<>& TankManagerComponent::OnLevelCompleted()
{
	return m_OnLevelCompleted;
}

void TankManagerComponent::CheckBounds(TankComponent* tank)
{
	const glm::ivec2 position{ tank->GetOwner()->GetLocalTransform().GetPosition() };
	glm::ivec2 newPosition{ position };

	if (position.x < 0)
	{
		newPosition.x = 0;
	}
	else if (position.x > m_TileManager->GetTileArraySize() * m_TileManager->GetTileSize())
	{
		newPosition.x = m_TileManager->GetTileArraySize() * m_TileManager->GetTileSize();		
	}
	else if (position.y < 0)
	{
		newPosition.y = 0;
	}
	else if (position.y > m_TileManager->GetTileArraySize() * m_TileManager->GetTileSize())
	{
		newPosition.y = m_TileManager->GetTileArraySize() * m_TileManager->GetTileSize();
	}

	tank->GetOwner()->SetLocalPosition(newPosition);
}

void TankManagerComponent::RemoveAllPlayers()
{
	for (TankComponent* tank : m_Tanks)
	{
		PlayerTankComponent* player{ dynamic_cast<PlayerTankComponent*>(tank) };
		if (player != nullptr)
		{
			player->SetStatus(ControllableObject::Status::Destroyed);
		}
	}
}

void TankManagerComponent::Update()
{
	if (!m_Alive) m_Alive = true;	
}

void TankManagerComponent::LateUpdate()
{
	CheckLevel();	
}

bool TankManagerComponent::Alive()
{
	return m_Alive;
}

void TankManagerComponent::CheckLevel()
{
	if (PlayerTankComponent::Counter() == 0)
	{
		m_OnGameOver.Notify();	
	}
	else if(EnemyTankComponent::Counter() == 0)
	{
		m_OnLevelCompleted.Notify();
	}
}