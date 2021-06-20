#pragma once

#include <map>

#include "core/maths/primitives.hpp"
#include "physics/transform.hpp"
#include "lowrenderer/model.hpp"

namespace Game
{
	enum class Tag
	{
		PLAYER,
		ENEMY,
		PLATFORM,
		UNASSIGNED
	};

	class GameObject
	{
	public:
		GameObject() = default;
		GameObject(const std::vector<float>& _transform, const std::string& colliderName, const std::vector<float>& colliderAttribs);
		~GameObject();
		GameObject(const GameObject& other);

		void						operator=(const GameObject& other);

		void 						addShape(Core::Maths::Primitives* shape);

		virtual void				setCollider(const std::string& colliderName, const std::vector<float>& colliderAttribs);

		void						addShader(const std::vector<std::string>& modelShaders, const Core::Maths::vec3& lightCounts);
		void						fillMesh(const std::vector<Resources::Mesh>& meshes);
		void						addMesh(const std::string& resourceInfo);
		void						addMesh();
		void						loadTextures(const std::vector<std::string>& materials, const std::vector<std::string>& texFiles,
										std::map<std::string, unsigned int>& cachedTextures
									);
		void						defineVAO();
		virtual void                showImGuiControls();

		Core::Maths::Primitives*	shape = nullptr;
		Physics::Transform			transform;
		LowRenderer::Model			model;
		Tag							tag = Tag::UNASSIGNED;
		std::string					customTexture = "";

		bool						selected = false;
		bool						destroying = false;
		bool						hasPrimShape = false;
	};
}