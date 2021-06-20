#pragma once

#include "game/gameobject.hpp"
#include "physics/rigidbody.hpp"

#include "game/platform.hpp"

namespace Game
{
	enum class State
	{
		GROUNDED,
		JUMPING
	};

	struct Input
	{
		bool	moveForward;
		bool	moveBackward;
		bool	moveLeft;
		bool	moveRight;
		bool	jump;
		int		jumpState;
	};

	class Entity : public GameObject
	{
		public:
			Entity() = default;
			Entity(const std::vector<float>& _transform);

			virtual void				update(const Input& inputs);

		protected:
			virtual void				calcHoriTranslation(const Input& inputs, std::vector<GameObject*>& gos);

			Physics::RigidBody	rigidBody;
	};
}