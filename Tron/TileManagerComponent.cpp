#include <exception>
#include <algorithm>

#include "TileManagerComponent.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Bullet.h"

TileManagerComponent::TileManagerComponent(Minigin::GameObject* owner, int tileSize) :
	Component{ owner },
	m_Tiles
	{
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} }
	},
	m_TileZero{ Minigin::Renderer::Instance()->CreateTexture(Minigin::ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Tile Zero.png") },
	m_TileOne{ Minigin::Renderer::Instance()->CreateTexture(Minigin::ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Tile One.png") },
	m_TileTwoCorner{ Minigin::Renderer::Instance()->CreateTexture(Minigin::ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Tile Two Corner.png") },
	m_TileTwoStraight{ Minigin::Renderer::Instance()->CreateTexture(Minigin::ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Tile Two Straight.png") },
	m_TileThree{ Minigin::Renderer::Instance()->CreateTexture(Minigin::ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Tile Three.png") },
	m_TileFour{ Minigin::Renderer::Instance()->CreateTexture(Minigin::ResourceManager::Instance()->GetTextureRootPath() / "Tiles/Tile Four.png") },
	m_TileSize{ tileSize },
	m_CollisionOffset{ 10 }
{
	CreateTiles();
}

glm::ivec2 TileManagerComponent::GetStartPosition() const
{
	glm::ivec2 position{ GetOwner()->GetWorldTransform().GetPosition() };

	return glm::ivec2{ position.x + static_cast<int>(m_TileSize * (m_Tiles.size() / 2)), position.y + static_cast<int>(m_TileSize * (m_Tiles.size() / 2)) };
}

bool TileManagerComponent::CanMove(TankComponent const* tank, MoveCommand::Direction direction) const
{
	bool canMove{ false };

	const glm::ivec2 worldTankPosition{ tank->GetWorldPosition() };
	const glm::ivec2 offset{ GetOwner()->GetWorldTransform().GetPosition() };

	const int row{ (worldTankPosition.y - offset.y) / m_TileSize };
	const int collumn{ (worldTankPosition.x - offset.x) / m_TileSize };

	switch (direction)
	{
	case MoveCommand::Direction::Up:
		if ((worldTankPosition.x - offset.x) % m_TileSize == 0)
		{
			// cross road
			if ((worldTankPosition.y - offset.y) % m_TileSize == 0)
			{
				// not top edge
				if (row < static_cast<int>(m_Tiles.size()))
				{
					// check if at right edge
					if (collumn == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row).at(collumn - 1).CanPass(Tile::Side::Left);
					else canMove = m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Left);
				}
			}
			// between two
			else
			{
				// check if at right edge
				if (collumn == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row).at(collumn - 1).CanPass(Tile::Side::Left);
				else canMove = m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Left);
			}
		}
		break;
	case MoveCommand::Direction::Right:
		if ((worldTankPosition.y - offset.y) % m_TileSize == 0)
		{
			// cross road
			if ((worldTankPosition.x - offset.x) % m_TileSize == 0)
			{
				// not right edge
				if (collumn < static_cast<int>(m_Tiles.size()))
				{
					// check if at top edge
					if (row == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row - 1).at(collumn).CanPass(Tile::Side::Bottom);
					else canMove = m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Bottom);
				}
			}
			// between two
			else
			{
				// check if at top edge
				if (row == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row - 1).at(collumn).CanPass(Tile::Side::Bottom);
				else canMove = m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Bottom);
			}
		}
		break;
	case MoveCommand::Direction::Down:
		if ((worldTankPosition.x - offset.x) % m_TileSize == 0)
		{
			// cross road
			if ((worldTankPosition.y - offset.y) % m_TileSize == 0)
			{
				// not bottom edge
				if (row > 0)
				{
					// check if at right edge
					if (collumn == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row - 1).at(collumn - 1).CanPass(Tile::Side::Left);
					else canMove = m_Tiles.at(row - 1).at(collumn).CanPass(Tile::Side::Left);
				}
			}
			// between two
			else
			{
				// check if at right edge
				if (collumn == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row).at(collumn - 1).CanPass(Tile::Side::Left);
				else canMove = m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Left);
			}
		}
		break;
	case MoveCommand::Direction::Left:
		if ((worldTankPosition.y - offset.y) % m_TileSize == 0)
		{
			// cross road
			if ((worldTankPosition.x - offset.x) % m_TileSize == 0)
			{
				// not left edge
				if (collumn > 0)
				{
					// check if at top edge
					if (row == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row - 1).at(collumn - 1).CanPass(Tile::Side::Bottom);
					else canMove = m_Tiles.at(row).at(collumn - 1).CanPass(Tile::Side::Bottom);
				}
			}
			// between two
			else
			{
				// check if at top edge
				if (row == static_cast<int>(m_Tiles.size())) canMove = m_Tiles.at(row - 1).at(collumn).CanPass(Tile::Side::Bottom);
				else canMove = m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Bottom);
			}
		}
		break;
	default:
		break;
	}

	return canMove;
}

bool TileManagerComponent::CheckCollision(Bullet* bullet) const
{
	bool removeBullet{ false };

	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };
	std::optional<glm::ivec2> intersection{};	// this is in tile manager space not world

	// We are inside the boundaries
	intersection = CheckBounds(bullet, removeBullet);
	if (!intersection)
	{
		// We did not hit the top collision zone of the tile some tiles have this
		intersection = CheckTop(bullet, removeBullet);	
		if (!intersection)	
		{
			// We did not hit the right collision zone of the tile some tiles have this
			intersection = CheckRight(bullet, removeBullet);	
			if (!intersection)
			{
				// We did not hit the bottom collision zone of the tile some tiles have this
				intersection = CheckBottom(bullet, removeBullet);
				if (!intersection)
				{
					// We did not hit the bottom collision zone of the tile some tiles have this
					intersection = CheckLeft(bullet, removeBullet);		
					if (!intersection)
					{
						// We did not hit the center collision zone of the current tile (every tile has one)
						intersection = CheckCenter(bullet, removeBullet);
						if (!intersection)
						{
							intersection;
						}
					}
				}
			}
		}
	}

	// Set position the bullet to the position of the intersection in world space
	if (intersection) bullet->SetPosition(intersection.value() + tileManagerOffset);

	return removeBullet;
}

void TileManagerComponent::Render() const
{
	glm::ivec2 position{ GetOwner()->GetWorldTransform().GetPosition() };
	const glm::ivec2 startPosition{ position.x + (m_TileSize / 2), position.y + (m_TileSize / 2) };
	std::pair<Minigin::Texture const*, Minigin::Transform> renderInfo{};

	for (int row{}; row < static_cast<int>(m_Tiles.size()); ++row)
	{
		for (int collumn{}; collumn < static_cast<int>(m_Tiles.at(row).size()); ++collumn)
		{
			renderInfo = GetRenderInfo(m_Tiles.at(row).at(collumn));

			renderInfo.second.SetPosition(glm::ivec2{ startPosition.x + (collumn * m_TileSize), startPosition.y + (row * m_TileSize) });

			renderInfo.first->Render(renderInfo.second);
		}
	}
}

std::pair<Minigin::Texture const*, Minigin::Transform> TileManagerComponent::GetRenderInfo(const Tile& tile) const
{
	std::pair<Minigin::Texture const*, Minigin::Transform> info{ std::make_pair<Minigin::Texture const*, Minigin::Transform>(nullptr, Minigin::Transform{}) };

	const int closedSides{ 4 - tile.GetTotalOpenSides() };

	switch (closedSides)
	{
	case 0:
		info.first = m_TileZero.get();
		info.second.SetScale(GetScale(m_TileZero.get()));

		break;
	case 1:
		info.first = m_TileOne.get();
		info.second.SetScale(GetScale(m_TileOne.get()));

		if (!tile.CanPass(Tile::Side::Bottom)) info.second.SetRotation(90);
		else if (!tile.CanPass(Tile::Side::Left)) info.second.SetRotation(180);
		else if (!tile.CanPass(Tile::Side::Top)) info.second.SetRotation(270);

		break;
	case 2:
		if ((tile.CanPass(Tile::Side::Top) and tile.CanPass(Tile::Side::Bottom)) or (tile.CanPass(Tile::Side::Right) and tile.CanPass(Tile::Side::Left)))
		{
			info.first = m_TileTwoStraight.get();
			info.second.SetScale(GetScale(m_TileTwoStraight.get()));

			if (!tile.CanPass(Tile::Side::Top)) info.second.SetRotation(90);
		}
		else
		{
			info.first = m_TileTwoCorner.get();
			info.second.SetScale(GetScale(m_TileTwoCorner.get()));

			if (!tile.CanPass(Tile::Side::Right) and !tile.CanPass(Tile::Side::Bottom)) info.second.SetRotation(90);
			else if (!tile.CanPass(Tile::Side::Bottom) and !tile.CanPass(Tile::Side::Left)) info.second.SetRotation(180);
			else if (!tile.CanPass(Tile::Side::Left) and !tile.CanPass(Tile::Side::Top)) info.second.SetRotation(270);
		}

		break;
	case 3:
		info.first = m_TileThree.get();
		info.second.SetScale(GetScale(m_TileThree.get()));

		if (tile.CanPass(Tile::Side::Left)) info.second.SetRotation(90);
		else if (tile.CanPass(Tile::Side::Top)) info.second.SetRotation(180);
		else if (tile.CanPass(Tile::Side::Right)) info.second.SetRotation(270);

		break;
	case 4:
		info.first = m_TileFour.get();
		info.second.SetScale(GetScale(m_TileFour.get()));

		break;
	default:
		throw std::exception{ "TileManagerComponent::GetTuleTexture() - Invalid amount of closed sides." };
		break;
	}

	return info;
}

glm::vec2 TileManagerComponent::GetScale(Minigin::Texture const* texture) const
{
	const glm::ivec2 textureSize{ texture->GetSize() };

	return glm::vec2{ static_cast<float>(m_TileSize) / textureSize.x, static_cast<float>(m_TileSize) / textureSize.y };
}

void TileManagerComponent::CreateTiles()
{
	std::array<std::array<Tile, 8>, 8> tiles
	{
		std::array<Tile, 8>{ Tile{ false, false, true, true }, Tile{ true, true, true, false }, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{ true, true, false, true }, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{ false, true, true, true }, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{ true, true, false, true }, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
		std::array<Tile, 8>{ Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{}, Tile{} },
	};

	m_Tiles = tiles;
}

bool TileManagerComponent::PointInsideRectangle(const glm::ivec2& point, const glm::ivec2& bottom, const glm::ivec2 top) const
{
	// Not too high
	if (point.y <= top.y)
	{
		// Not too right
		if (point.x <= top.x)
		{
			// Not too low
			if (point.y >= bottom.y)
			{
				// Not too left
				if (point.x >= bottom.x)
				{
					return true;
				}
			}
		}
	}

	return false;
}

std::optional<glm::ivec2> TileManagerComponent::LinesIntersect(const glm::ivec2& a, const glm::ivec2& b, const glm::ivec2& c, const glm::ivec2& d) const
{
	std::optional<glm::ivec2> intersection{};	

	const glm::vec2 directionOne{ b - a }; // Direction vector of the first line segment				
	const glm::vec2 directionTwo{ d - c }; // Direction vector of the second line segment			

	const float determinant{ directionOne.x * directionTwo.y - directionOne.y * directionTwo.x }; // Determinant		
	const float cross{ (c - a).x * directionOne.y - (c - a).y * directionOne.x }; // Cross product of vectors			

	if (determinant == 0.0f)	
	{
		return intersection; // Lines are parallel or collinear
	}

	const float t{ cross / determinant };	
	const float u{ ((c - a).x * directionTwo.y - (c - a).y * directionTwo.x) / determinant };	

	if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)	
	{
		intersection = a + glm::ivec2{ t * directionOne };	
		return intersection;	
	}
	else return intersection;	
}

std::optional<glm::ivec2> TileManagerComponent::CheckBounds(Bullet* bullet, bool& removeBullet) const
{
	std::optional<glm::ivec2> intersection{};
	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };
	const glm::ivec2 bulletTileManagerPosition{ bullet->GetPosition() - tileManagerOffset }; // tile manager space	

	// Did we hit or pass the top boundry
	if (bulletTileManagerPosition.y >= int(m_TileSize * m_Tiles.size()))
	{
		intersection = glm::ivec2{ bulletTileManagerPosition.x, int(m_TileSize * m_Tiles.size()) - 1 };
		removeBullet = bullet->Bounce(Tile::Side::Top);
	}
	// Did we hit or pass the right boundry
	else if (bulletTileManagerPosition.x >= int(m_TileSize * m_Tiles.size()))
	{
		intersection = glm::ivec2{ int(m_TileSize * m_Tiles.size()) - 1, bulletTileManagerPosition.y };
		removeBullet = bullet->Bounce(Tile::Side::Right);
	}
	// Did we hit or pass the bottom boundry
	else if (bulletTileManagerPosition.y < 0)
	{
		intersection = glm::ivec2{ bulletTileManagerPosition.x, 0 };
		removeBullet = bullet->Bounce(Tile::Side::Bottom);
	}
	// Did we hit or pass the left boundry
	else if (bulletTileManagerPosition.x < 0)
	{
		intersection = glm::ivec2{ 0, bulletTileManagerPosition.y };
		removeBullet = bullet->Bounce(Tile::Side::Left);
	}

	return intersection;
}

std::optional<glm::ivec2> TileManagerComponent::CheckCenter(Bullet* bullet, bool& removeBullet) const
{
	std::optional<glm::ivec2> intersection{};	// tile space

	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };
	const glm::ivec2 bulletTileManagerPosition{ bullet->GetPosition() - tileManagerOffset };	// tile manager space

	const int row{ bulletTileManagerPosition.y / m_TileSize };
	const int collumn{ bulletTileManagerPosition.x / m_TileSize };

	const glm::ivec2 tileOffset{ (collumn * m_TileSize) , (row * m_TileSize) };
	const glm::ivec2 bulletTilePosition{ bulletTileManagerPosition - tileOffset }; // tile space			

	const glm::ivec2 bottomLeft{ m_CollisionOffset, m_CollisionOffset };	// tile space
	const glm::ivec2 topRight{ m_TileSize - m_CollisionOffset - 1, m_TileSize - m_CollisionOffset - 1 };	// tile space	

	// Is point in default collision center box of a tile all tiles have this
	if (PointInsideRectangle(bulletTilePosition, bottomLeft, topRight))
	{
		const glm::ivec2 bottomRight{ topRight.x, bottomLeft.y };	// tile space
		const glm::ivec2 topLeft{ bottomLeft.x, topRight.y };	// tile space

		// Start point of line we check for intersection will be 100 units before bullet was inside the rectangle
		const glm::ivec2 start{ bulletTilePosition - glm::ivec2{ bullet->GetDirection() * 100.0f } };

#pragma warning (push)
#pragma warning (disable : 4706) // C4706 assignment used as a condition

		// Did it pass the top side if so where
		if (intersection = LinesIntersect(topLeft, topRight, start, bulletTilePosition))
		{
			removeBullet = bullet->Bounce(Tile::Side::Top);
		}
		// Did it pass the right side if so where
		else if (intersection = LinesIntersect(topRight, bottomRight, start, bulletTilePosition))
		{
			removeBullet = bullet->Bounce(Tile::Side::Right);
		}
		// Did it pass the bottom side if so where
		else if (intersection = LinesIntersect(bottomLeft, bottomRight, start, bulletTilePosition))
		{
			removeBullet = bullet->Bounce(Tile::Side::Bottom);
		}
		// Has to have passed the left but where
		else
		{
			if (intersection = LinesIntersect(bottomLeft, topLeft, start, bulletTilePosition))
			{
				removeBullet = bullet->Bounce(Tile::Side::Left);	
			}
			else
			{
				throw std::exception{ "TileManagerComponent::CheckCenter() - Bullet is in center collision zone but couldn't find entry side" };	
			}
		}
#pragma warning (pop)
	}

	// tile space to tile manager space
	if (intersection) intersection.value() += tileOffset;	

	return intersection;
}

std::optional<glm::ivec2> TileManagerComponent::CheckTop(Bullet* bullet, bool& removeBullet) const
{
	std::optional<glm::ivec2> intersection{};	// tile space	

	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };	
	const glm::ivec2 bulletTileManagerPosition{ bullet->GetPosition() - tileManagerOffset };	// tile manager space	

	const int row{ bulletTileManagerPosition.y / m_TileSize };	
	const int collumn{ bulletTileManagerPosition.x / m_TileSize };

	const glm::ivec2 tileOffset{ (collumn * m_TileSize) , (row * m_TileSize) };	
	const glm::ivec2 bulletTilePosition{ bulletTileManagerPosition - tileOffset }; // tile space		

	if (!m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Top))
	{
		const glm::ivec2 bottomLeft{ m_CollisionOffset, m_TileSize - m_CollisionOffset };	// tile space
		const glm::ivec2 topRight{ m_TileSize - m_CollisionOffset - 1, m_TileSize - 1 };	// tile space

		// Is point in default collision center box of a tile all tiles have this
		if (PointInsideRectangle(bulletTilePosition, bottomLeft, topRight))	
		{
			const glm::ivec2 bottomRight{ topRight.x, bottomLeft.y };
			const glm::ivec2 topLeft{ bottomLeft.x, topRight.y };

			// Start point of line we check for intersection will be 100 units before bullet was inside the rectangle
			const glm::ivec2 start{ bulletTilePosition - glm::ivec2{ bullet->GetDirection() * 100.0f } };	

#pragma warning (push)
#pragma warning (disable : 4706) // C4706 assignment used as a condition

			// Did it pass the top side if so where
			if (intersection = LinesIntersect(topLeft, topRight, start, bulletTilePosition))	
			{
				removeBullet = bullet->Bounce(Tile::Side::Top);
			}
			// Did it pass the right side if so where
			else if (intersection = LinesIntersect(topRight, bottomRight, start, bulletTilePosition))	
			{
				removeBullet = bullet->Bounce(Tile::Side::Right);
			}
			// Has to have passed the left but where
			else	
			{
				if (intersection = LinesIntersect(bottomLeft, topLeft, start, bulletTilePosition))
				{
					removeBullet = bullet->Bounce(Tile::Side::Left);
				}
			}
#pragma warning (pop)
		}
	}

	// tile space to tile manager space
	if (intersection) intersection.value() += tileOffset;

	return intersection;
}

std::optional<glm::ivec2> TileManagerComponent::CheckRight(Bullet* bullet, bool& removeBullet) const
{
	std::optional<glm::ivec2> intersection{};	// tile space	

	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };
	const glm::ivec2 bulletTileManagerPosition{ bullet->GetPosition() - tileManagerOffset };	// tile manager space	

	const int row{ bulletTileManagerPosition.y / m_TileSize };
	const int collumn{ bulletTileManagerPosition.x / m_TileSize };

	const glm::ivec2 tileOffset{ (collumn * m_TileSize) , (row * m_TileSize) };
	const glm::ivec2 bulletTilePosition{ bulletTileManagerPosition - tileOffset }; // tile space		

	if (!m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Right))	
	{
		const glm::ivec2 bottomLeft{ m_TileSize - m_CollisionOffset, m_CollisionOffset };	// tile space
		const glm::ivec2 topRight{ m_TileSize - 1, m_TileSize - m_CollisionOffset - 1 };	// tile space

		// Is point in default collision center box of a tile all tiles have this
		if (PointInsideRectangle(bulletTilePosition, bottomLeft, topRight))
		{
			const glm::ivec2 bottomRight{ topRight.x, bottomLeft.y };
			const glm::ivec2 topLeft{ bottomLeft.x, topRight.y };

			// Start point of line we check for intersection will be 100 units before bullet was inside the rectangle
			const glm::ivec2 start{ bulletTilePosition - glm::ivec2{ bullet->GetDirection() * 100.0f } };

#pragma warning (push)
#pragma warning (disable : 4706) // C4706 assignment used as a condition

			// Did it pass the top side if so where
			if (intersection = LinesIntersect(topLeft, topRight, start, bulletTilePosition))
			{
				removeBullet = bullet->Bounce(Tile::Side::Top);
			}
			// Did it pass the right side if so where
			else if (intersection = LinesIntersect(topRight, bottomRight, start, bulletTilePosition))
			{
				removeBullet = bullet->Bounce(Tile::Side::Right);
			}
			// Has to have passed the bottom but where
			else
			{	
				if (intersection = LinesIntersect(bottomLeft, bottomRight, start, bulletTilePosition))	
				{
					removeBullet = bullet->Bounce(Tile::Side::Bottom);
				}
			}
#pragma warning (pop)
		}
	}

	// tile space to tile manager space
	if (intersection) intersection.value() += tileOffset;

	return intersection;
}

std::optional<glm::ivec2> TileManagerComponent::CheckBottom(Bullet* bullet, bool& removeBullet) const
{
	std::optional<glm::ivec2> intersection{};	// tile space	

	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };
	const glm::ivec2 bulletTileManagerPosition{ bullet->GetPosition() - tileManagerOffset };	// tile manager space	

	const int row{ bulletTileManagerPosition.y / m_TileSize };
	const int collumn{ bulletTileManagerPosition.x / m_TileSize };

	const glm::ivec2 tileOffset{ (collumn * m_TileSize) , (row * m_TileSize) };
	const glm::ivec2 bulletTilePosition{ bulletTileManagerPosition - tileOffset }; // tile space		

	if (!m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Bottom))
	{
		const glm::ivec2 bottomLeft{ m_CollisionOffset, 0 };	// tile space
		const glm::ivec2 topRight{ m_TileSize - m_CollisionOffset - 1, m_CollisionOffset - 1 };	// tile space

		// Is point in default collision center box of a tile all tiles have this
		if (PointInsideRectangle(bulletTilePosition, bottomLeft, topRight))
		{
			const glm::ivec2 bottomRight{ topRight.x, bottomLeft.y };
			const glm::ivec2 topLeft{ bottomLeft.x, topRight.y };

			// Start point of line we check for intersection will be 100 units before bullet was inside the rectangle
			const glm::ivec2 start{ bulletTilePosition - glm::ivec2{ bullet->GetDirection() * 100.0f } };

#pragma warning (push)
#pragma warning (disable : 4706) // C4706 assignment used as a condition

			// Did it pass the right side if so where
			if (intersection = LinesIntersect(topRight, bottomRight, start, bulletTilePosition))	
			{
				removeBullet = bullet->Bounce(Tile::Side::Right);
			}
			// Did it pass the bottom side if so where
			else if (intersection = LinesIntersect(bottomLeft, bottomRight, start, bulletTilePosition))	
			{
				removeBullet = bullet->Bounce(Tile::Side::Bottom);
			}
			// Has to have passed the left but where
			else
			{
				if (intersection = LinesIntersect(bottomLeft, topLeft, start, bulletTilePosition))
				{
					removeBullet = bullet->Bounce(Tile::Side::Left);	
				}
			}
#pragma warning (pop)
		}
	}

	// tile space to tile manager space
	if (intersection) intersection.value() += tileOffset;

	return intersection;
}

std::optional<glm::ivec2> TileManagerComponent::CheckLeft(Bullet* bullet, bool& removeBullet) const
{
	std::optional<glm::ivec2> intersection{};	// tile space	

	const glm::ivec2 tileManagerOffset{ GetOwner()->GetWorldTransform().GetPosition() };
	const glm::ivec2 bulletTileManagerPosition{ bullet->GetPosition() - tileManagerOffset };	// tile manager space	

	const int row{ bulletTileManagerPosition.y / m_TileSize };
	const int collumn{ bulletTileManagerPosition.x / m_TileSize };

	const glm::ivec2 tileOffset{ (collumn * m_TileSize) , (row * m_TileSize) };
	const glm::ivec2 bulletTilePosition{ bulletTileManagerPosition - tileOffset }; // tile space		

	if (!m_Tiles.at(row).at(collumn).CanPass(Tile::Side::Left))	
	{
		const glm::ivec2 bottomLeft{ 0, m_CollisionOffset };	// tile space
		const glm::ivec2 topRight{ m_CollisionOffset - 1, m_TileSize - m_CollisionOffset - 1 };	// tile space

		// Is point in default collision center box of a tile all tiles have this
		if (PointInsideRectangle(bulletTilePosition, bottomLeft, topRight))
		{
			const glm::ivec2 bottomRight{ topRight.x, bottomLeft.y };
			const glm::ivec2 topLeft{ bottomLeft.x, topRight.y };

			// Start point of line we check for intersection will be 100 units before bullet was inside the rectangle
			const glm::ivec2 start{ bulletTilePosition - glm::ivec2{ bullet->GetDirection() * 100.0f } };

#pragma warning (push)
#pragma warning (disable : 4706) // C4706 assignment used as a condition

			// Did it pass the top side if so where
			if (intersection = LinesIntersect(topLeft, topRight, start, bulletTilePosition))
			{
				removeBullet = bullet->Bounce(Tile::Side::Top);
			}
			// Did it pass the Left side if so where
			else if (intersection = LinesIntersect(bottomLeft, topLeft, start, bulletTilePosition))		
			{
				removeBullet = bullet->Bounce(Tile::Side::Left);		
			}
			// Has to have passed the bottom but where
			else
			{
				if (intersection = LinesIntersect(bottomLeft, bottomRight, start, bulletTilePosition))
				{
					removeBullet = bullet->Bounce(Tile::Side::Bottom);
				}
			}
#pragma warning (pop)
		}
	}

	// tile space to tile manager space
	if (intersection) intersection.value() += tileOffset;

	return intersection;
}
