#pragma once

#include <string>

#include "GameObject.h"
#include "ObjectController.h"

namespace Minigin
{
	class Scene final : public ControllableObject, public ObjectController<GameObject>
	{
	public:
		explicit Scene(const std::string& name);
		~Scene() = default;

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) noexcept = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) noexcept = delete;

		GameObject* CreateGameObject(bool enabled = true);
		const std::string& GetName() const;

	private:
		const std::string m_Name;

	};
}