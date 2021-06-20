#pragma once

#include <vector>
#include <map>
#include <string>

#include "resources/resourcesmanager.hpp"
#include "resources/scene.hpp"

namespace Core
{
	namespace DataStructure
	{
		class Graph
		{
		public:
			Graph() = default;
			
			Graph(const Graph& graph) = delete;

			void operator=(const Graph& graph) = delete;

			inline Resources::Scene&		getScene(int index) { return scenes[index]; }

			void							loadScenes();
			void							unloadScenes();
			void							saveScenes();
			void							loadSavedScene();
			void							setScenes();
			bool							hasSavedScene();

			Resources::ResourcesManager		rm;

			bool							scenesLoaded = false;

		private:
			void							parseSceneList(std::vector<std::string>& sceneList, const char* filePath) const;
			void							loadScene(const bool saved);
			bool endOfLine(int iplus1, std::string& line);
			void editLightCounts();
			void							saveScene();
			void							loadModels(std::map<std::string, std::vector<float>>& models, 
												std::vector<std::string>& modelNames, 
												std::map<std::string, std::vector<float>>& colliders,
												const std::vector<std::string>& colliderNames,
												const std::vector<std::string>& shaderInfo,
												std::map<int, std::vector<int>>& gameObjects,
												const std::vector<std::string>& customTextures
											);

			std::vector<Resources::Scene>	scenes;
		};
	}
}