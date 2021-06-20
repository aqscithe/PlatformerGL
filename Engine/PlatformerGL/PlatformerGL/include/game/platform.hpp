#pragma once

#include "game/gameobject.hpp"
#include "core/maths/primitives.hpp"
#include "physics/transform.hpp"
#include "lowrenderer/model.hpp"

namespace Game
{
	class Platform : public virtual GameObject
	{
	public:
		Platform(
			const std::vector<float>& transformInfo, const std::string& colliderName, const std::vector<float>& colliderAttribs,
			const std::vector<int>& gameObjAttrib, const std::string& customTexture
		);
	};
}