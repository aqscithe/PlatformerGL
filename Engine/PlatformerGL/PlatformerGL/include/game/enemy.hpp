#pragma once

#include "game/entity.hpp"

namespace Game
{
	class Enemy : public virtual Entity
	{
		public:
			Enemy(
				const std::vector<float>& transformInfo, const std::string& colliderName, const std::vector<float>& colliderAttribs,
				const std::vector<int>& gameObjAttrib, const std::string& customTexture
			);

			void				showImGuiControls() override;
			
			int					damage;

		private:
			Core::Maths::vec3	target;
	};
}