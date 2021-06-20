#include <fstream>

#include "resources/resourcesmanager.hpp"
#include "physics/transform.hpp"
#include "core/debug/log.hpp"
#include "core/debug/assertion.hpp"


using namespace Resources;

void ResourcesManager::addResource(const int resourceType, const std::vector<std::string>& modelShaders)
{
    auto& scene = scenes.back();
    Core::Maths::vec3 lightCounts = { scene.dirLights.size(), scene.pointLights.size(), scene.spotLights.size() };
    switch (latestTag)
    {
        case static_cast<int>(Game::Tag::PLAYER):
            scene.players.back().addShader(modelShaders, lightCounts);
            break;
        case static_cast<int>(Game::Tag::ENEMY):
            scene.enemies.back().addShader(modelShaders, lightCounts);
            break;
        case static_cast<int>(Game::Tag::PLATFORM) :
            scene.platforms.back().addShader(modelShaders, lightCounts);
            break;
        default:
                    
            break;
    }

    std::string statement = "Added shader";
    Core::Debug::Log::print(statement, Core::Debug::LogType::INFO);
}

void ResourcesManager::addResource(const int resourceType, const std::string& resourceInfo, const bool collider)
{  
    switch (latestTag)
    {
        case static_cast<int>(Game::Tag::PLAYER) :
            if (collider)
                scenes.back().players.back().addMesh();
            else
                scenes.back().players.back().addMesh(resourceInfo);
            break;
        case static_cast<int>(Game::Tag::ENEMY) :
            if (collider)
                scenes.back().enemies.back().addMesh();
            else
                scenes.back().enemies.back().addMesh(resourceInfo);
            break;
        case static_cast<int>(Game::Tag::PLATFORM) :
            if (collider)
                scenes.back().platforms.back().addMesh();
            else
                scenes.back().platforms.back().addMesh(resourceInfo);
            break;
        default:
            break;
    }
}

void ResourcesManager::addResource(const int resourceType, const std::string& resourceInfo)
{
    Core::Debug::LogType logType;
    std::string statement;

    switch (resourceType)
    {
        case static_cast<int>(ResourceType::SCENE) :
            scenes.push_back(Resources::Scene(resourceInfo));
            logType = Core::Debug::LogType::INFO;
             statement = "Added SCENE";
            Core::Debug::Log::print(statement, logType);
            break;
        default:
            statement = "ResourcesManager::addResource() 1 - Invalid resource type: "
                + std::to_string(resourceType);
            logType = Core::Debug::LogType::ERROR;
            Core::Debug::Log::print(statement, logType);
            break;
    }
}

void ResourcesManager::addResource(const int resourceType, const int value)
{
    Core::Debug::LogType logType;
    std::string statement;

	switch (resourceType)
	{
        case static_cast<int>(ResourceType::TEXTURE) :
            statement = "Added texture";
            logType = Core::Debug::LogType::INFO;
            Core::Debug::Log::print(statement, logType);
			break;
		default:
            logType = Core::Debug::LogType::ERROR;
			std::string statement = "ResourcesManager::addResource() 2 - Invalid resource type: "
				+ std::to_string(resourceType);
			Core::Debug::Log::print(statement, logType);
			break;
	}
}

void ResourcesManager::loadObj(
        std::string& modelName, const std::vector<float>& modelObjAttribs, const std::string& colliderName, 
        const std::vector<float>& colliderAttribs, const std::vector<std::string>& modelShaders, const std::vector<int>& gameObjAttribs,
        const std::string& customTexture
)
{
    for (size_t i = modelName.length() - 1; i > 0; --i)
    {
        if (modelName[i] == '_')
            modelName.pop_back();
        else
            break;
    }

    std::string filepath = "Assets/models/" + modelName + ".obj";
    const char* modelPath = filepath.c_str();
    std::string filepath2 = "Assets/colliders/" + colliderName + ".obj";
    const char* colliderPath = filepath2.c_str();

    scenes.back().addGameObject(modelObjAttribs, colliderName, colliderAttribs, gameObjAttribs, customTexture);
    latestTag = int(modelObjAttribs.back());

    addResource(static_cast<int>(Resources::ResourceType::SHADER), modelShaders);
    if (loadObj(modelPath, colliderPath, modelName))
    {
        switch (latestTag)
        {
            case static_cast<int>(Game::Tag::PLAYER) :
                for (Resources::Mesh& mesh : scenes.back().players.back().model.meshes)
                {
                    mesh.setIndices();
                }
                break;
            case static_cast<int>(Game::Tag::ENEMY) :
                for (Resources::Mesh& mesh : scenes.back().enemies.back().model.meshes)
                {
                    mesh.setIndices();
                }
                break;
            case static_cast<int>(Game::Tag::PLATFORM) :
                for (Resources::Mesh& mesh : scenes.back().platforms.back().model.meshes)
                {
                    mesh.setIndices();
                }
            default:
                break;
        }
        
    }
    loadMaterials();
    
}

bool ResourcesManager::loadObj(const char* modelFile, const char* colliderFile, const std::string& modelName)
{
    using namespace Core::Maths;

    if (cachedModelMeshes.count(modelName) > 0 && cachedModelType.count(modelName) > 0)
    {
        loadCachedGO(modelName);
        return true;
    }
    else
    {
        // ASSERT
        Core::Debug::Assertion::assertTest(!std::string(modelFile).empty());
        Core::Debug::Assertion::assertTest(!std::string(colliderFile).empty());

        std::vector<const char*> files = { modelFile, colliderFile };

        std::vector<Mesh>		meshes;

        int meshesStart = int(meshes.size());
        int meshesEnd = meshesStart;

        for (const char* file : files)
        {
            std::ifstream readFile;
            readFile.open(file, std::ios::in);


            if (readFile.is_open())
            {
                std::string line;

                std::vector<vec3> vertices;
                std::vector<vec2> texCoords;
                std::vector<vec3> normals;
                bool addedTexture = false;
                bool hasMTL = false;
                int faceCount = 0;

                while (std::getline(readFile, line))
                {
                    if (line[0] != 'v' && line[0] != 'f' && line[0] != 'u' && line[0] != 'm')
                        continue;

                    std::string category;
                    std::string text;
                    bool categoryFound = false;
                    bool faceCountIncremented = false;
                    bool addedVertexCount = false;
                    vec3 in;
                    int count = 0;
                    int specs[12];

                    for (int i = 0; i < line.length(); ++i)
                    {
                        int endOfLine = i + 1;
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
                            if (faceCount > 0 && category == "v")
                            {
                                addResource(static_cast<int>(Resources::ResourceType::TEXTURE), faceCount * 3);
                                faceCount = 0;
                            }

                            if (category == "v" && addedTexture)
                            {
                                addedTexture = false;
                            }

                            if (category == "mtllib" && c != '/')
                            {
                                text += c;
                            }
                            else if (c != ' ' && c != '/')
                            {
                                text += c;
                            }

                            if (category == "v" || category == "vt" || category == "vn")
                            {


                                if ((i + 1) == (int)line.length() || line[endOfLine] == ' ')
                                {
                                    count++;
                                    if (count == 1)
                                        in.x = std::stof(text);
                                    else if (count == 2)
                                        in.y = std::stof(text);
                                    else if (count == 3)
                                        in.z = std::stof(text);
                                    text = "";
                                }

                                if (category == "v" && count == 3)
                                {
                                    vertices.push_back(in);
                                    in = { 0.f, 0.f, 0.f };
                                    count = 0;
                                }
                                else if (category == "vt" && count == 2)
                                {
                                    texCoords.push_back(in.xy);
                                    in = { 0.f, 0.f, 0.f };
                                    count = 0;
                                }
                                else if (category == "vn" && count == 3)
                                {
                                    normals.push_back(in);
                                    in = { 0.f, 0.f, 0.f };
                                    count = 0;
                                }
                                continue;
                            }
                            else if (category == "f")
                            {
                                if (!hasMTL && faceCount == 0)
                                {
                                    std::string temp = "";
                                    addResource(static_cast<int>(Resources::ResourceType::MESH), temp, true);
                                    cachedMTLNames.emplace(modelName, text);
                                    cachedModelType.emplace(modelName, true);
                                    switch (latestTag)
                                    {
                                        case static_cast<int>(Game::Tag::PLAYER) :
                                            meshes.push_back(scenes.back().players.back().model.meshes.back());
                                            break;
                                        case static_cast<int>(Game::Tag::ENEMY) :
                                            meshes.push_back(scenes.back().enemies.back().model.meshes.back());
                                            break;
                                        case static_cast<int>(Game::Tag::PLATFORM) :
                                            meshes.push_back(scenes.back().platforms.back().model.meshes.back());
                                            break;
                                        default:
                                            break;
                                    }
                                    ++meshesEnd;
                                }

                                if (!faceCountIncremented)
                                {
                                    ++faceCount;
                                    faceCountIncremented = true;
                                }

                                if (((i + 1) == (int)line.length() && text != "") || line[endOfLine] == ' ' || line[endOfLine] == '/')
                                {
                                    specs[count] = std::stoi(text) - 1;
                                    count++;
                                    text = "";
                                }
                                if (count == 9 && (i + 1) == (int)line.length()) // triangle face
                                {
                                    if (meshes.back().faceType != FaceType::TRIANGLE)
                                        meshes.back().faceType = FaceType::TRIANGLE;
                                    vec3 vertex[3] = { vertices.at(specs[0]), vertices.at(specs[3]), vertices.at(specs[6]) };
                                    vec2 texCoord[3] = { texCoords.at(specs[1]), texCoords.at(specs[4]), texCoords.at(specs[7]) };
                                    vec3 normal[3] = { normals.at(specs[2]), normals.at(specs[5]), normals.at(specs[8]) };
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[0].x, vertex[0].y, vertex[0].z,
                                        1.f, 0.f, 0.f, 1.f,
                                        normal[0].x, normal[0].y, normal[0].z,
                                        texCoord[0].x, texCoord[0].y,
                                        });
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[1].x, vertex[1].y, vertex[1].z,
                                        0.f, 1.f, 0.f, 1.f,
                                        normal[1].x, normal[1].y, normal[1].z,
                                        texCoord[1].x, texCoord[1].y,
                                        });
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[2].x, vertex[2].y, vertex[2].z,
                                        0.f, 0.f, 1.f, 1.f,
                                        normal[2].x, normal[2].y, normal[2].z,
                                        texCoord[2].x, texCoord[2].y,
                                        });
                                    count = 0;
                                }
                                else if (count == 12) // 4-vertex face
                                {
                                    if (meshes.back().faceType != FaceType::QUAD)
                                        meshes.back().faceType = FaceType::QUAD;
                                    vec3 vertex[4] = { vertices.at(specs[0]), vertices.at(specs[3]), vertices.at(specs[6]), vertices.at(specs[9]) };
                                    vec2 texCoord[4] = { texCoords.at(specs[1]), texCoords.at(specs[4]), texCoords.at(specs[7]), texCoords.at(specs[10]) };
                                    vec3 normal[4] = { normals.at(specs[2]), normals.at(specs[5]), normals.at(specs[8]), normals.at(specs[11]) };
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[0].x, vertex[0].y, vertex[0].z,
                                        1.f, 0.f, 0.f, 1.f,
                                        normal[0].x, normal[0].y, normal[0].z,
                                        texCoord[0].x, texCoord[0].y,
                                        });
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[1].x, vertex[1].y, vertex[1].z,
                                        0.f, 1.f, 0.f, 1.f,
                                        normal[1].x, normal[1].y, normal[1].z,
                                        texCoord[1].x, texCoord[1].y,
                                        });
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[2].x, vertex[2].y, vertex[2].z,
                                        0.f, 0.f, 1.f, 1.f,
                                        normal[2].x, normal[2].y, normal[2].z,
                                        texCoord[2].x, texCoord[2].y,
                                        });
                                    meshes.back().rdrVertices.push_back(Core::rdrVertex{
                                        vertex[3].x, vertex[3].y, vertex[3].z,
                                        0.f, 0.f, 1.f, 1.f,
                                        normal[3].x, normal[3].y, normal[3].z,
                                        texCoord[3].x, texCoord[3].y,
                                        });

                                    count = 0;
                                }
                            }
                            else if ((i + 1) == (int)line.length())
                            {
                                if (category == "mtllib")
                                {
                                    hasMTL = true;
                                    cachedMTLFiles.emplace(modelName, text);

                                }
                                else if (hasMTL && category == "usemtl")
                                {
                                    addResource(static_cast<int>(Resources::ResourceType::MESH), text, false);
                                    cachedMTLNames.emplace(modelName, text);
                                    cachedModelType.emplace(modelName, false);
                                    switch (latestTag)
                                    {
                                        case static_cast<int>(Game::Tag::PLAYER) :
                                            meshes.push_back(scenes.back().players.back().model.meshes.back());
                                            break;
                                        case static_cast<int>(Game::Tag::ENEMY) :
                                            meshes.push_back(scenes.back().enemies.back().model.meshes.back());
                                            break;
                                        case static_cast<int>(Game::Tag::PLATFORM) :
                                            meshes.push_back(scenes.back().platforms.back().model.meshes.back());
                                            break;
                                        default:

                                            break;
                                    }
                                    ++meshesEnd;
                                }
                            }
                        }
                    }
                }
                readFile.close();
                if (faceCount > 0)
                {
                    addResource(static_cast<int>(Resources::ResourceType::TEXTURE), faceCount * 3);
                    faceCount = 0;
                }
            }
            else
            {
                std::string statement = "Unable to open file: " + std::string(file);
                Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
                return false;
            }
        }

        cachedModelMeshes.emplace(modelName, std::vector<Resources::Mesh>(meshes.begin() + meshesStart, meshes.begin() + meshesEnd));


        loadCachedGO(modelName);

        return true;
    }
    return false;
}

void Resources::ResourcesManager::loadCachedGO(const std::string& modelName)
{
    switch (latestTag)
    {
        case static_cast<int>(Game::Tag::PLAYER) :
            scenes.back().players.back().model.materialsFile = cachedMTLFiles[modelName];
            addResource(static_cast<int>(Resources::ResourceType::MESH), cachedMTLNames[modelName], cachedModelType[modelName]);
            scenes.back().players.back().fillMesh(cachedModelMeshes[modelName]);
            break;
        case static_cast<int>(Game::Tag::ENEMY) :
            scenes.back().enemies.back().model.materialsFile = cachedMTLFiles[modelName];
            addResource(static_cast<int>(Resources::ResourceType::MESH), cachedMTLNames[modelName], cachedModelType[modelName]);
            scenes.back().enemies.back().fillMesh(cachedModelMeshes[modelName]);
            break;
        case static_cast<int>(Game::Tag::PLATFORM) :
            scenes.back().platforms.back().model.materialsFile = cachedMTLFiles[modelName];
            addResource(static_cast<int>(Resources::ResourceType::MESH), cachedMTLNames[modelName], cachedModelType[modelName]);
            scenes.back().platforms.back().fillMesh(cachedModelMeshes[modelName]);
            break;
        default:
            break;
    }
}


void ResourcesManager::loadMaterials()
{
    std::vector<std::string> materials;
    std::vector<std::string> texFiles;
 
    parseMtl(materials, texFiles);
    loadTextures(materials, texFiles);
}


void ResourcesManager::parseMtl(std::vector<std::string>& materials, std::vector<std::string>& texFiles)
{

    std::string path = "";
    
    switch (latestTag)
    {
        case static_cast<int>(Game::Tag::PLAYER) :
            Core::Debug::Assertion::assertTest(!scenes.back().players.back().model.materialsFile.empty());
            path = "Assets/mtls/" + scenes.back().players.back().model.materialsFile;
            break;
        case static_cast<int>(Game::Tag::ENEMY) :
            Core::Debug::Assertion::assertTest(!scenes.back().enemies.back().model.materialsFile.empty());
            path = "Assets/mtls/" + scenes.back().enemies.back().model.materialsFile;
            break;
        case static_cast<int>(Game::Tag::PLATFORM) :
            Core::Debug::Assertion::assertTest(!scenes.back().platforms.back().model.materialsFile.empty());
            path = "Assets/mtls/" + scenes.back().platforms.back().model.materialsFile;
            break;
        default:
            break;
    }
    
    const char* filename = path.c_str();

    std::string statement = "Materials file: " + path;
    Core::Debug::Log::print(statement, Core::Debug::LogType::DEBUG);
    

    std::ifstream readFile;
    readFile.open(filename, std::ios::in);
    if (readFile.is_open())
    {
        std::string line;

        while (std::getline(readFile, line))
        {
            if (line[0] != 'n' && line[0] != 'm')
                continue;

            std::string category;
            std::string text;
            bool categoryFound = false;
            bool atIndex0 = true;
            int count = 0;

            for (int i = 0; i < line.length(); ++i)
            {
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
                    if (c != ' ' && atIndex0)
                    {
                        text += c;
                        atIndex0 = false;
                    }
                    else
                    {
                        text += c;
                    }

                    if ((i + 1) == (int)line.length())
                    {
                        if (category == "newmtl")
                        {
                            materials.push_back(text);
                        }
                        else if (category == "map_Kd")
                        {
                            for (int i = 0; i < text.length(); ++i)
                            {
                                if (text[i] == '\\')
                                {
                                    int iplus1 = i + 1;
                                    text = text.substr(iplus1, text.length() - (iplus1));
                                    i = 0;
                                }
                            }
                            texFiles.push_back(text);
                        }
                    }
                }
            }
        }
        readFile.close();
    }
    else
    {
        std::string statement = "Unable to open file: " + std::string(filename);
        Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
    }
}


void	ResourcesManager::loadTextures(const std::vector<std::string>& materials, const std::vector<std::string>& texFiles)
{
    switch (latestTag)
    {
        case static_cast<int>(Game::Tag::PLAYER) :
            scenes.back().players.back().loadTextures(materials, texFiles, cachedTextures);
            break;
        case static_cast<int>(Game::Tag::ENEMY) :
            scenes.back().enemies.back().loadTextures(materials, texFiles, cachedTextures);
            break;
        case static_cast<int>(Game::Tag::PLATFORM) :
            scenes.back().platforms.back().loadTextures(materials, texFiles, cachedTextures);
            break;
        default:
            break;
    }
}



