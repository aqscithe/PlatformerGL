#include <fstream>
#include <sstream>

#include "resources/shader.hpp"
#include "core/debug/log.hpp"

using namespace Resources;

Shader::Shader(const std::string& vertexFilename, const std::string& fragFilename, const Core::Maths::vec3& lightCounts)
{
	vertexShaderString = "Bin/shaders/" + vertexFilename;
	fragShaderString = "Bin/shaders/" + fragFilename;

	initShader();
	getShaderSources(lightCounts);
	initShaderProgram();
}

Shader::Shader(const std::string& vertexFilename, const std::string& fragFilename)
{
	vertexShaderString = "Bin/shaders/" + vertexFilename;
	fragShaderString = "Bin/shaders/" + fragFilename;

	initShader();
	getShaderSources();
	initShaderProgram();
}

Shader::Shader(const Shader& other)
{
	*this = other;
}

void Shader::operator=(const Shader& other)
{
	vertexShader = other.vertexShader;
	fragmentShader = other.fragmentShader;
	shaderProgram = other.shaderProgram;
	fragShaderString = other.fragShaderString;
	vertexShaderString = other.vertexShaderString;
}

void	Shader::initShader()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
}


void	Shader::initShaderProgram()
{
	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragShaderSource = fragShaderString.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::string statement = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog);
		Core::Debug::Log::print( statement, Core::Debug::LogType::ERROR);
	}

	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::string statement = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog);
		Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::string statement = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog);
		Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void	Shader::getShaderSources()
{
	std::ifstream vertexFile(vertexShaderString);
	std::ifstream fragmentFile(fragShaderString);

	verifyFiles(vertexFile, fragmentFile);

	std::string vertexFileContent((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
	std::string fragFileContent((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());

	vertexFile.close();
	fragmentFile.close();

	vertexShaderString = vertexFileContent;
	fragShaderString = fragFileContent;
}

void	Shader::getShaderSources(const Core::Maths::vec3& lightCounts)
{
	std::ifstream vertexFile(vertexShaderString);
	std::ifstream fragmentFile(fragShaderString);

	verifyFiles(vertexFile, fragmentFile);

	std::string vertexFileContent((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
	std::string fragFileContent((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());

	vertexFile.close();
	fragmentFile.close();


	std::string replacing = "#define NR_DIR_LIGHTS 00";
	int pos = 0;
	std::string dirLightCount = " " + std::to_string(int(lightCounts.x)) + "\n";
	setShaderLightCount(fragFileContent, replacing, pos, dirLightCount);

	replacing = "#define NR_POINT_LIGHTS 00";
	std::string pointLightCount = " " + std::to_string(int(lightCounts.y));
	setShaderLightCount(fragFileContent, replacing, pos, pointLightCount);

	replacing = "#define NR_SPOT_LIGHTS 00";
	std::string spotLightCount = " " + std::to_string(int(lightCounts.z));
	setShaderLightCount(fragFileContent, replacing, pos, spotLightCount);

	vertexShaderString = vertexFileContent;
	fragShaderString = fragFileContent;
}

void Resources::Shader::setShaderLightCount(std::string& fragFileContent, std::string& replacing, int& pos, const std::string& lightCount)
{
	int subStrIndex = 0;
	for (int i = pos; i < fragFileContent.size(); ++i)
	{
		if (fragFileContent[i] == replacing[subStrIndex])
		{
			if (subStrIndex == replacing.size() - 1)
			{
				pos = i - 1;
				break;
			}
			++subStrIndex;
		}
		else
			subStrIndex = 0;
	}
	fragFileContent.replace(pos, lightCount.size(), lightCount);
}

void Resources::Shader::verifyFiles(std::ifstream& vertexFile, std::ifstream& fragmentFile)
{
	std::string statement = "Vertex shader: " + vertexShaderString;
	Core::Debug::Log::print(statement, Core::Debug::LogType::DEBUG);
	statement = "Fragment shader: " + fragShaderString;
	Core::Debug::Log::print(statement, Core::Debug::LogType::DEBUG);

	if (!vertexFile.is_open() || !fragmentFile.is_open())
	{
		std::string statement = "A Shader file failed to open";
		Core::Debug::Log::print(statement, Core::Debug::LogType::ERROR);
	}
}

void	Shader::setMat4(const std::string& name, const Core::Maths::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_TRUE, (GLfloat*)value.e);
}

void	Shader::setVec3(const std::string& name, const Core::Maths::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, (GLfloat*)value.e);
}

void	Shader::setVec4(const std::string& name, const Core::Maths::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, (GLfloat*)value.e);
}

void	Shader::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), (GLfloat)value);
}

void	Shader::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (GLint)value);
}

void	Shader::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (GLboolean)value);
}
