#ifndef INPUT_MAPPING_CONTEXT
#define INPUT_MAPPING_CONTEXT

#include <type_traits>

#include "InputAction.h"
#include "unordered_set"

class InputMappingContext
{
public:
	InputMappingContext(GameObject* gameObject);
	~InputMappingContext() = default;

	InputMappingContext(const InputMappingContext&) = delete;
	InputMappingContext(InputMappingContext&&) noexcept = default;
	InputMappingContext& operator= (const InputMappingContext&) = delete;
	InputMappingContext& operator= (const InputMappingContext&&) = delete;

	std::unordered_set<InputAction>& GetInputActions();

    bool operator==(const InputMappingContext& rhs) const;

    template<typename CommandType>
    bool AddInputAction(SDL_KeyCode keyCode)
    {
        if constexpr (std::is_base_of<GameObjectCommand, CommandType>::value)
        {
            if (m_LinkedToGameObjet)
            {
                return m_InputActions.emplace(keyCode, new CommandType{ m_GameObject }).second;
            }
            else return false;
        }
        else
        {
            if (!m_LinkedToGameObjet)
            {
                return m_InputActions.emplace(keyCode, new CommandType()).second;
            }
            else return false;
        }
    }

    friend struct std::hash<InputMappingContext>;

private:
	GameObject* m_GameObject;
	const bool m_LinkedToGameObjet;
	std::unordered_set<InputAction> m_InputActions;

};

template<>
struct std::hash<InputMappingContext>
{
    std::size_t operator()(const InputMappingContext& mapping) const;
};

#endif