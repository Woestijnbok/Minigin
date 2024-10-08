#pragma once

#include <memory>

#include "TankComponent.h"
#include "Texture.h"

class EnemyTankComponent : public TankComponent
{
public:
	explicit EnemyTankComponent(Minigin::GameObject* owner, TankManagerComponent* manager, const std::shared_ptr<Minigin::Texture>& tankTexture, float speed);	
	virtual ~EnemyTankComponent();

	EnemyTankComponent(const EnemyTankComponent&) = delete;
	EnemyTankComponent(EnemyTankComponent&&) noexcept = delete;
	EnemyTankComponent& operator= (const EnemyTankComponent&) = delete;
	EnemyTankComponent& operator= (const EnemyTankComponent&&) noexcept = delete;

	Minigin::Subject<int>& OnDie();

	virtual glm::vec2 GetFireDirection() const override;
	virtual void Hit() override;
	virtual void Render() const override;

	static int Counter();

protected:
	bool CheckPlayers();
	void Roam();

private:
	std::shared_ptr<Minigin::Texture> m_TankTexture;
	Minigin::Subject<int> m_OnDie;
	int m_Lives;

	void SetRandomDirection();
	bool CanSeePlayer(const glm::ivec2& tankPosition, const glm::ivec2& playerPosition, MoveCommand::Direction direction) const;

	static int m_Counter;
};