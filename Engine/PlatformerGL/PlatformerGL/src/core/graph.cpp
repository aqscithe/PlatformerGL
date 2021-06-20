#include <iostream>
#include <fstream>

#include "core/datastructure/graph.hpp"
#include "core/debug/log.hpp"
#include "core/debug/assertion.hpp"

#include "lowrenderer/directionallight.hpp"
#include "lowrenderer/spotlight.hpp"
#include "lowrenderer/pointlight.hpp"

#include "core/maths/sphere.hpp"
#include "core/maths/box.hpp"
#include "core/debug/memleaks.hpp"

#include "time.hpp"

using namespace Core::DataStructure;

void Graph::loadScenes()
{
	std::vector<std::string> sceneList;
	parseSceneList(sceneList, "Bin/scenes/scene_list.txt");
	for (std::string& scene : sceneList)
	{
        rm.addResource(static_cast<int>(Resources::ResourceType::SCENE), scene);
        loadScene(false);
	}

    {
        std::string statement = "Scenes Loaded: " + std::to_string(rm.scenes.size());
        Core::Debug::Log::print(statement, Core::Debug::LogType::INFO);
        for (Resources::Scene& scene : rm.scenes)
        {
            scene.debug();
        }
    }
}

void Graph::unloadScenes()
{
    rm.scenes.clear();
    scenes.clear();

    scenesLoaded = false;
}

void Graph::saveScenes()
{
    saveScene();
}

bool Graph::hasSavedScene()
{
    std::vector<std::string> sceneList;
    parseSceneList(sceneList, "Bin/scenes/scene_list.txt");
    for (std::string& scene : sceneList)
    {
        //looking for a scene with "save_"
        std::string path = "Bin/scenes/save_" + scene;
        std::ifstream savedFile;
        savedFile.open(path);

        if (savedFile.is_open())
        {
            savedFile.close();
            return true;
        }
        else
        {
            savedFile.close();
            return false;
        }
    }
    return false;
}

void Graph::loadSavedScene()
{
    std::vector<std::string> sceneList;
    parseSceneList(sceneList, "Bin/scenes/scene_list.txt");
    for (std::string& scene : sceneList)
    {
        rm.addResource(static_cast<int>(Resources::ResourceType::SCENE), scene);
        loadScene(true);
    }

    {
        std::string statement = "Scenes Loaded: " + std::to_string(rm.scenes.size());
        Core::Debug::Log::print(statement, Core::Debug::LogType::INFO);
        for (Resources::Scene& scene : rm.scenes)
        {
            scene.debug();
        }
    }
}

void Graph::setScenes()
{
    scenes = rm.scenes;
    for (Resources::Scene& scene : scenes)
        scene.setGameObjects();

    scenesLoaded = true;

    Time::resetLastTime();
}

void Graph::parseSceneList(std::vector<std::string>& sceneList, const char* filePath) const
{
	std::string path = std::string(filePath);
	std::ifstream readFile;
	readFile.open(path, std::ios::in);

    if (readFile.is_open())
    {
        std::string line;

        while (std::getline(readFile, line))
        {
            if(line[0] != '#')
                sceneList.push_back(line);
        }
    }
    else
    {
        std::string statement = "Unable to open file: " + std::string(path);
        Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
    }
}

void Graph::loadScene(const bool saved)
{
    std::string path;
    if (!saved)
        path = "Bin/scenes/" + rm.scenes.back().name;
    else
        path = "Bin/scenes/save_" + rm.scenes.back().name;

    std::ifstream readFile;
    readFile.open(path, std::ios::in);

    std::map<std::string, std::vector<float>> models;
    std::map<std::string, std::vector<float>> colliders;
    std::map<int, std::vector<int>> gameObjects;

    std::vector<int> tags;
    std::vector<int> attributes;

    std::vector<std::string> customTextures;

    std::vector<std::string> shaderInfo;
    std::vector<std::string> modelNames;
    std::vector<std::string> colliderNames;
    std::vector<float> colliderAttribs;
    std::vector<float> transform;
    std::vector<float> cameraInfo;

    std::vector<float> pointLightInfo;
    std::vector<float> dirLightInfo;
    std::vector<float> spotLightInfo;


    bool modelNameAdded = false;
    bool colliderNameAdded = false;
    bool tagAdded = false;

    if (readFile.is_open())
    {
        std::string line;

        while (std::getline(readFile, line))
        {
            bool categoryFound = false;
            std::string category;
            std::string text;

            for (int i = 0; i < line.length(); ++i)
            {
                int iplus1 = i + 1;
                char c = line[i];
                if (!categoryFound)
                {
                    if (c == ' ')
                    {
                        categoryFound = true;
                    }
                    else
                    {
                        category += c;
                        continue;
                    }
                }
                else
                {
                    if (category == "s")
                    {
                        if (c != ' ')
                            text += c;
                        if (endOfLine(iplus1, line) || line[iplus1] == ' ')
                        {
                            shaderInfo.push_back(text);
                            text.erase();
                        }
                    }
                    else if (category == "m")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if (!modelNameAdded && line[iplus1] == ' ')
                        {
                            modelNames.push_back(text);
                            text.erase();
                            modelNameAdded = true;
                        }
                        else if ( line[iplus1] == ' ' || line[iplus1] == '/')
                        {
                            transform.push_back(std::stof(text));
                            text.erase();
                        }
                        else if (endOfLine(iplus1, line))
                        {
                            transform.push_back(std::stof(text));
                            std::string name = modelNames.back();
                            while (models.count(name) > 0)
                            {
                                name += "_";
                            }
                            modelNames.back() = name;
                            models.emplace(name, transform);
                            text.erase();
                            transform.clear();
                            modelNameAdded = false;
                        }
                    }
                    else if (category == "t")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if (endOfLine(iplus1, line))
                        {
                            customTextures.push_back(text);
                        }
                    }
                    else if (category == "co")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if (!colliderNameAdded && line[iplus1] == ' ')
                        {
                            colliderNames.push_back(text);
                            text.erase();
                            colliderNameAdded = true;
                        }
                        else if (line[iplus1] == ' ' || line[iplus1] == '/')
                        {
                            colliderAttribs.push_back(std::stof(text));
                            text.erase();
                        }
                        else if (endOfLine(iplus1, line))
                        {
                            colliderAttribs.push_back(std::stof(text));
                            std::string name = colliderNames.back();
                            while (colliders.count(name) > 0)
                            {
                                name += "_";
                            }
                            colliders.emplace(modelNames.back(), colliderAttribs);
                            colliderNames.back() = name;
                            text.erase();
                            colliderAttribs.clear();
                            colliderNameAdded = false;
                        }
                    }
                    else if (category == "c")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if (endOfLine(iplus1, line) || line[iplus1] == ' ' || line[iplus1] == '/')
                        {
                            cameraInfo.push_back(std::stof(text));
                            text.erase();
                        }
                    }
                    else if (category == "ld" || category == "lp" || category == "ls")
                    {
                        if (c != ' ' && c != '/')
                            text += c;
                        if (line[iplus1] == ' ' || line[iplus1] == '/')
                        {
                            char light = category.back();
                            switch (light)
                            {
                                case 'd':
                                    dirLightInfo.push_back(std::stof(text));
                                    break;
                                case 's':
                                    spotLightInfo.push_back(std::stof(text));
                                    break;
                                case 'p':
                                    pointLightInfo.push_back(std::stof(text));
                                    break;
                                default:
                                    break;
                            }
                            text.erase();
                        }
                        else if (endOfLine(iplus1, line))
                        {
                            char light = category.back();
                            switch (light)
                            {
                                case 'd':
                                    dirLightInfo.push_back(std::stof(text));
                                    rm.scenes.back().dirLights.push_back(LowRenderer::DirectionalLight(dirLightInfo));
                                    dirLightInfo.clear();
                                    break;
                                case 's':
                                    spotLightInfo.push_back(std::stof(text));
                                    rm.scenes.back().spotLights.push_back(LowRenderer::SpotLight(spotLightInfo));
                                    spotLightInfo.clear();
                                    break;
                                case 'p':
                                    pointLightInfo.push_back(std::stof(text));
                                    rm.scenes.back().pointLights.push_back(LowRenderer::PointLight(pointLightInfo));
                                    pointLightInfo.clear();
                                    break;
                                default:
                                    break;
                            }
                            text.erase();
                        }

                    }
                    else if (category == "0" || category == "1" || category == "2")
                    {
                        if (!tagAdded)
                        {
                            switch (std::stoi(category))
                            {
                                case static_cast<int>(Game::Tag::PLAYER):
                                    tags.push_back(static_cast<int>(Game::Tag::PLAYER));
                                    break;
                                case static_cast<int>(Game::Tag::ENEMY) :
                                    tags.push_back(static_cast<int>(Game::Tag::ENEMY));
                                    break;
                                case static_cast<int>(Game::Tag::PLATFORM) :
                                    tags.push_back(static_cast<int>(Game::Tag::PLATFORM));
                                    break;
                                default:
                                    tags.push_back(static_cast<int>(Game::Tag::UNASSIGNED));
                                    break;
                            }
                            text.erase();
                            tagAdded = true;
                        }
                        if (c != ' ' && c != '/')
                            text += c;
                        if (line[iplus1] == ' ' || line[iplus1] == '/')
                        {
                            attributes.push_back(std::stoi(text));
                            text.erase();
                        }
                        else if ((i + 1) == (int)line.length())
                        {
                            attributes.push_back(std::stoi(text));
                            gameObjects.emplace(tags.back(), attributes);
                            text.erase();
                            attributes.clear();
                            tagAdded = false;
                        }
                    }
                }
            }
        }
        readFile.close();
    }
    else
    {
        std::string statement = "Unable to open file: " + std::string(path);
        Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
    }
    

    editLightCounts();

    
    
        
    loadModels(models, modelNames, colliders, colliderNames, shaderInfo, gameObjects, customTextures);
    rm.scenes.back().camera = LowRenderer::Camera(1280, 720, cameraInfo);
}

bool Core::DataStructure::Graph::endOfLine(int iplus1, std::string& line)
{
    return (iplus1) == line.length();
}

void Core::DataStructure::Graph::editLightCounts()
{
    // ensuring that there is always 1 enabled dir light in the scene
    if (rm.scenes.back().dirLights.size() == 0)
    {
        rm.scenes.back().dirLights.push_back(LowRenderer::DirectionalLight());
        rm.scenes.back().dirLights.back().enabled = true;
    }
    

}

void Graph::saveScene()
{  
    std::string path = "Bin/scenes/" + rm.scenes.back().name;
    std::ifstream sceneFile;
    sceneFile.open(path, std::ios::in);

    if (!sceneFile.is_open())
        return;

    std::string savePath = "Bin/scenes/save_" + rm.scenes.back().name;
    std::ofstream saveFile(savePath);

    if (!saveFile.is_open())
    {
        sceneFile.close();
        return;
    }

    std::string line;

    int gameObjectType = 0;
    std::cout << "saving" << std::endl;
    int playerCount = 0;
    int enemyCount = 0;
    int platformCount = 0;
    while (std::getline(sceneFile, line))
    {
        switch (line[0])
        {
        case 'm':
            if (line[1] != ' ')
                break;

            saveFile << line[0] << line[1];
            for (int i = 2; line[i] != ' '; ++i)
            {
                saveFile << line[i];
            }

            if (line.back() == '0')         //player
            {
                gameObjectType = 0;
                Physics::Transform tr = scenes.back().players[playerCount++].transform;

                saveFile << ' ' << tr.position.x;
                saveFile << ' ' << tr.position.y;
                saveFile << ' ' << tr.position.z << '/';

                saveFile << tr.rotation.x;
                saveFile << ' ' << tr.rotation.y;
                saveFile << ' ' << tr.rotation.z << '/';

                saveFile << tr.scale.x;
                saveFile << ' ' << tr.scale.y;
                saveFile << ' ' << tr.scale.z << " 0\n";
            }
            else if (line.back() == '1')    //enemy
            {
                gameObjectType = 1;
                Physics::Transform tr = scenes.back().enemies[enemyCount++].transform;

                saveFile << ' ' << tr.position.x;
                saveFile << ' ' << tr.position.y;
                saveFile << ' ' << tr.position.z << '/';

                saveFile << tr.rotation.x;
                saveFile << ' ' << tr.rotation.y;
                saveFile << ' ' << tr.rotation.z << '/';

                saveFile << tr.scale.x;
                saveFile << ' ' << tr.scale.y;
                saveFile << ' ' << tr.scale.z << " 1\n";
            }
            else                            //platform
            {
                gameObjectType = 2;
                Physics::Transform tr = scenes.back().platforms[platformCount++].transform;

                saveFile << ' ' << tr.position.x;
                saveFile << ' ' << tr.position.y;
                saveFile << ' ' << tr.position.z << '/';

                saveFile << tr.rotation.x;
                saveFile << ' ' << tr.rotation.y;
                saveFile << ' ' << tr.rotation.z << '/';

                saveFile << tr.scale.x;
                saveFile << ' ' << tr.scale.y;
                saveFile << ' ' << tr.scale.z << " 2\n";
            }

            break;
        case 'c':
            if (line[1] == 'o')
            {
                if (line[2] != ' ')
                    break;

                saveFile << line[0] << line[1] << line[2];
                for (int i = 3; line[i] != ' '; ++i)
                {
                    saveFile << line[i];
                }

                if (gameObjectType == 0)
                {
                    if (!scenes.back().players.back().shape)
                    {
                        saveFile << '\n';
                        break;
                    }

                    if (!scenes.back().players.back().shape->sph)
                    {
                        saveFile << '\n';
                        break;
                    }

                    Core::Maths::Sphere sph = *scenes.back().players.back().shape->sph;

                    saveFile << ' ' << sph.omega.x;
                    saveFile << ' ' << sph.omega.y;
                    saveFile << ' ' << sph.omega.z << '/';

                    saveFile << sph.radius;
                    saveFile << ' ' << sph.radius;
                    saveFile << ' ' << sph.radius << '\n';
                }
                else if (gameObjectType == 1)
                {
                    if (!scenes.back().enemies.back().shape)
                    {
                        saveFile << '\n';
                        break;
                    }

                    if (!scenes.back().enemies.back().shape->sph)
                    {
                        saveFile << '\n';
                        break;
                    }

                    Core::Maths::Sphere sph = *scenes.back().enemies.back().shape->sph;

                    saveFile << ' ' << sph.omega.x;
                    saveFile << ' ' << sph.omega.y;
                    saveFile << ' ' << sph.omega.z << '/';

                    saveFile << sph.radius;
                    saveFile << ' ' << sph.radius;
                    saveFile << ' ' << sph.radius << '\n';
                }
                else
                {
                    if (!scenes.back().platforms.back().shape)
                    {
                        saveFile << '\n';
                        break;
                    }

                    if (!scenes.back().platforms.back().shape->b)
                    {
                        saveFile << '\n';
                        break;
                    }

                    Core::Maths::Box b = *scenes.back().platforms.back().shape->b;

                    saveFile << ' ' << b.center.x;
                    saveFile << ' ' << b.center.y;
                    saveFile << ' ' << b.center.z << '/';

                    saveFile << b.extensions.x;
                    saveFile << ' ' << b.extensions.y;
                    saveFile << ' ' << b.extensions.z << '\n';
                }
            }
            else if (line[1] == ' ')
            {
                saveFile << line[0] << line[1];

                LowRenderer::Camera cam = scenes.back().camera;

                saveFile << cam.getCamInfo()[0] << ' ';
                saveFile << cam.getCamInfo()[1] << ' ';
                saveFile << cam.getCamInfo()[2] << '/';
                saveFile << cam.getCamInfo()[3] << ' ';
                saveFile << cam.getCamInfo()[4] << '/';
                saveFile << cam.getCamInfo()[5] << ' ';
                saveFile << cam.getCamInfo()[6] << ' ';
                saveFile << cam.getCamInfo()[7] << '/';
                saveFile << cam.getCamInfo()[8] << ' ';
                saveFile << cam.getCamInfo()[9] << '\n';
            }

            break;
        case 's':
            if (line[1] != ' ')
                break;

            saveFile << line << '\n';

            break;
        case 'l':
            if (line[1] != 'd')
                break;

            saveFile << line << '\n';

            break;
        case '0':
        {
            if (line[1] != ' ')
                break;

            int health = scenes.back().players.back().getHealth();
            saveFile << line[0] << line[1] << health <<'\n';

            break;
        }
        case '1':
            if (line[1] != ' ')
                break;

            saveFile << line << '\n';

            break;
        case '2':
            if (line[1] != ' ')
                break;

            saveFile << line << '\n';

            break;
        default:
            saveFile << line << (sceneFile.eof() ? '\0' : '\n');
            break;
        }
    }

    std::cout << "saved" << std::endl;
    sceneFile.close();
    saveFile.close();
}

void	Graph::loadModels(std::map<std::string, std::vector<float>>& models, std::vector<std::string>& modelNames, 
    std::map<std::string, std::vector<float>>& colliders, const std::vector<std::string>& colliderNames,
    const std::vector<std::string>& shaderInfo, std::map<int, std::vector<int>>& gameObjects,
    const std::vector<std::string>& customTextures)
{
    // ASSERT
    Core::Debug::Assertion::assertTest(models.size() == modelNames.size());
    Core::Debug::Assertion::assertTest(models.size() == colliders.size());
    Core::Debug::Assertion::assertTest(models.size() == colliderNames.size());
    Core::Debug::Assertion::assertTest(shaderInfo.size() == models.size() * 4);

    int index = 0;
    for (std::string& modelName : modelNames)
    {
        int i = index * 4;
        int j = i + 1;
        int k = j + 1;
        int l = k + 1;
        std::vector<std::string> modelShaders = { shaderInfo[i], shaderInfo[j], shaderInfo[k], shaderInfo[l] };
        rm.loadObj(modelName, models[modelName], colliderNames[index], colliders[modelName],  modelShaders, gameObjects[models[modelName].back()], customTextures[index]);
        ++index;
    }
}
