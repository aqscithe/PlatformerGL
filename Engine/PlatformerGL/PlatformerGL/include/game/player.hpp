#pragma once

#include "game/entity.hpp"

namespace Game
{
	using namespace Core::Maths;

	class Player : public virtual Entity
	{
		public:
			Player(
				const std::vector<float>& transformInfo, const std::string& colliderName, const std::vector<float>& colliderAttribs, 
				const std::vector<int>& gameObjAttrib, const std::string& customTexture
			);

            void		update(const Input& playerInputs, std::vector<GameObject*>& gos);
			void		heal(const int h);
			void		takeDamage(const int damage);
			void		showImGuiControls() override;

			int			getHealth() const;

		private:
			void		groundPlayer(const vec3& normal);

			void		updateColliderPos();

			void		processCollisions(std::vector<GameObject*>& gos, vec3& normal, const bool initiateJump);

			//calculate next horizontal position
			void		calcHoriTranslation(const Input& inputs, std::vector<GameObject*>& gos);
			void		calcHoriNetVelocity(const Game::Input& inputs, vec3& netVelocity);
			//calculate next vertical position and assign it
			void		calcVertTranslation();

			//apply force for vertical movement (up and down)
			void		verticalMove(bool& initiateJump);
			//assign horizontal position
			void		horizontalMove(const vec3& normal);

			bool		jump(const Input& inputs);
			void		deteriorateJumpForce();
			
			float		initialJumpForce = 80.f;
			float		jumpForce = 0.f;
			float		moveForce = 40.f;
			int			maxHealth;
			int			health;
			float		speed;
			State		state;
	};
}