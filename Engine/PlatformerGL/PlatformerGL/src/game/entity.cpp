#include "game/entity.hpp"

using namespace Game;
using namespace Core::Maths;

Entity::Entity(const std::vector<float>& _transform)
{
    transform.position = vec3{ _transform[0], _transform[1], _transform[2] };
    transform.rotation = vec3{ _transform[3], _transform[4], _transform[5] };
    transform.scale = vec3{ _transform[6], _transform[7], _transform[8] };

    rigidBody = Physics::RigidBody(transform.position);
}

void	Entity::calcHoriTranslation(const Input& inputs, std::vector<GameObject*>& gos)
{

}

void	Entity::update(const Input& inputs)
{

}