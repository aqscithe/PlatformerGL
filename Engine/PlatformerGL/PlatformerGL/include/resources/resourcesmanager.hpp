#pragma once

#include <map>

#include "resources/shader.hpp"
#include "resources/mesh.hpp"
#include "resources/scene.hpp"
#include "resources/texture.hpp"

namespace Resources
{
	enum class ResourceType
	{
		MESH = 1,
		TEXTURE = 2,
		SHADER = 3,
		SCENE = 4
	};

	class ResourcesManager
	{
	public:
		ResourcesManager() = default;

		ResourcesManager(const ResourcesManager& other) = delete;
		void					operator=(const ResourcesManager& other) = delete;

		void					addResource(
									const int resourceType, const std::vector<std::string>& modelShaders
								);
		void					addResource(
									const int resourceType, const std::string& resourceInfo, 
									const bool collider
								);
		void					addResource(
									const int resourceType, const std::string& resourceInfo
								);
		void					addResource(const int resourceType, const int value);
		void					loadObj(std::string& modelName, const std::vector<float>& modelAttrib, 
									const std::string& colliderName, const std::vector<float>& colliderAttrib,
									const std::vector<std::string>& modelShaders, const std::vector<int>& gameObjAttrib,
									const std::string& customTexture
								);

		std::vector<Scene>		scenes;
		unsigned int			count = 0;
		int						latestTag = 0;

	private:
		bool					loadObj(const char* modelFile, const char* colliderFile, const std::string& modelName);

		void					loadCachedGO(const std::string& modelName);
		void					loadMaterials();
		void					parseMtl(std::vector<std::string>& materials, std::vector<std::string>& texFiles);
		void					loadTextures(const std::vector<std::string>& materials, const std::vector<std::string>& texFiles);


		std::map<std::string, std::vector<Resources::Mesh>> cachedModelMeshes;
		std::map<std::string, std::string>					cachedMTLFiles;
		std::map<std::string, std::string>					cachedMTLNames;
		std::map<std::string, bool>							cachedModelType;

		std::map<std::string, unsigned int>					cachedTextures;
	};
}