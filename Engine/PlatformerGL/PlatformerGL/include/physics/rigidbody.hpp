#pragma once

#include "core/maths/maths.hpp"
#include "core/maths/segment.hpp"

#define G -9.81f

namespace Physics
{
	using namespace Core::Maths;
	class RigidBody
	{
		public:
			RigidBody() = default;
			RigidBody(vec3& position);

			void		 showImGuiControls();
						 
			void		 applyForce(const vec3& force);
			void		 applyVelocity(vec3& v, const float& resistance);
						 
			inline float getMass() const { return mass; }
			inline float getFriction() const { return friction; }
			inline float getAirResistance() const { return airResistance; }
 						 
			inline vec3	 getVelocity() const { return velocity; }
			inline vec3	 getAcceleration() const { return acceleration; }
			void		 resetAcceleration();
						 
			void		 supportReaction(const vec3& normal);
			//update the segment tn
			void		 update(const vec3& position);
						 
			bool		 useGravity = true;
			bool		 useScalarSpeed = false;
						 
			float		 gravity = G;

			//segment from actual position to next position
			Segment		 tn = { { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f } };
						 
		private:		 
			vec3		 velocity = vec3{ 0.f, 0.f, 0.f };
			vec3		 acceleration = vec3{ 0.f, 0.f, 0.f };
			float		 mass = 1.f;
						 
			float		 friction = 7.50f;
			float		 airResistance = 1.55f;
	};
}