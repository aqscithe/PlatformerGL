#pragma once

#include <string>

#include <glad/glad.h>

#include "core/core.hpp"
#include "core/maths/maths.hpp"

namespace Resources
{
	class Shader
	{
	public:
		Shader() = default;
		Shader(const std::string& vertexFilename, const std::string& fragFilename, const Core::Maths::vec3& lightCounts);
		Shader(const std::string& vertexFilename, const std::string& fragFilename);
		Shader(const Shader& other); 

		void            operator=(const Shader& other);

		void			setMat4(const std::string& name, const Core::Maths::mat4& value) const;
		void			setVec3(const std::string& name, const Core::Maths::vec3& value) const;
		void			setVec4(const std::string& name, const Core::Maths::vec4& value) const;
		void			setFloat(const std::string& name, const float value) const;
		void			setInt(const std::string& name, const int value) const;
		void			setBool(const std::string& name, const bool value) const;

		GLuint			vertexShader = 0;  // DELETE
		GLuint			fragmentShader = 0;  // DELETE
		GLuint			shaderProgram = 0;

	private:
		void			initShader();
		void			getShaderSources();
		void			getShaderSources(const Core::Maths::vec3& lightCounts);
		void			setShaderLightCount(std::string& fragFileContent, std::string& replacing,  int& pos, const std::string& lightCount);
		void			verifyFiles(std::ifstream& vertexFile, std::ifstream& fragmentFile);
		void			initShaderProgram();

		std::string		fragShaderString = "";
		std::string		vertexShaderString = "";
	};
}