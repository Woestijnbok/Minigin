#pragma once

#include <array>
#include <memory>
#include <utility>
#include <vec2.hpp>
#include <optional>
#include <filesystem>

#include "Component.h"
#include "Texture.h"
#include "MoveCommand.h"

class BulletManagerComponent;
class TankManagerComponent;
class TileComponent;	
class BulletComponent;
class MiddleTileComponent;
class TankComponent;

class TileManagerComponent final : public Minigin::Component
{
public:	
	explicit TileManagerComponent(Minigin::GameObject* owner, int tileSize, const std::filesystem::path& path);	
	virtual ~TileManagerComponent();

	TileManagerComponent(const TileManagerComponent&) = delete;
	TileManagerComponent(TileManagerComponent&&) noexcept = delete;
	TileManagerComponent& operator= (const TileManagerComponent&) = delete;
	TileManagerComponent& operator= (const TileManagerComponent&&) noexcept = delete;

	void SetManagers(BulletManagerComponent* bulletManager, TankManagerComponent* tankManager);
	glm::ivec2 GetRandomPosition() const;	
	bool CanMove(TankComponent* tank, MoveCommand::Direction direction) const;
	bool CanMove(const glm::ivec2& position, MoveCommand::Direction direction) const;
	void CheckCollision(BulletComponent* bullet) const;
	int GetTileSize() const;
	int GetTileArraySize() const;
	void RemoveTile(TileComponent* tile);
	
	virtual void Update() override;
	virtual void Render() const override;

	static bool Alive();

private:
	std::array<std::array<TileComponent*, 8>, 8> m_Tiles;
	MiddleTileComponent* m_MiddleTile;
	const std::unique_ptr<Minigin::Texture> m_TileZero;		
	const std::unique_ptr<Minigin::Texture> m_TileOne;	
	const std::unique_ptr<Minigin::Texture> m_TileTwoCorner;	
	const std::unique_ptr<Minigin::Texture> m_TileTwoStraight;	
	const std::unique_ptr<Minigin::Texture> m_TileThree;	
	const std::unique_ptr<Minigin::Texture> m_TileFour;
	const std::unique_ptr<Minigin::Texture> m_Border;
	const int m_TileSize;
	const int m_CollisionOffset;
	BulletManagerComponent* m_BulletManager;
	TankManagerComponent* m_TankManager;

	static bool m_Alive;

	int GetRotation(TileComponent* tile) const;		
	glm::vec2 GetScale(TileComponent* tile) const;	
	void CreateTiles();
	void CreateTiles(const std::filesystem::path& path);
	void CreateTile(int row, int collumn, char character);
	void CreateMiddleTile();
	std::optional<glm::ivec2> CheckBounds(BulletComponent* bullet) const;	
	std::optional<glm::ivec2> CheckCenter(BulletComponent* bullet) const;
	std::optional<glm::ivec2> CheckTop(BulletComponent* bullet) const;	
	std::optional<glm::ivec2> CheckRight(BulletComponent* bullet) const;	
	std::optional<glm::ivec2> CheckBottom(BulletComponent* bullet) const;	
	std::optional<glm::ivec2> CheckLeft(BulletComponent* bullet) const;	
};