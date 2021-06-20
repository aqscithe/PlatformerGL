#pragma once

#include <string>

namespace Resources
{
	class Texture
	{
	public:
		Texture();
		Texture(const Texture& other);

		void                operator=(const Texture& other);

		unsigned int		bindTexture();
		void				processTexData(const std::string& textureFile);

		unsigned int		texCount;
		int					width;
		int					height;

	private:
		void				debug(const std::string& textureFile);
		unsigned char*      loadImage(std::string& filename, int& width, int& height);
	};
}