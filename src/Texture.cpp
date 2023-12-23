#include <glad/glad.h>
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string_view path) :
	textureID(0), textureFilePath(path), textureLocalBuffer(nullptr), textureWidth(0), textureHeight(0) {
	std::string_view sub_str = path.substr(path.find_last_of('.') + 1);
	nrChannels = (sub_str == "png") ? 4 : 3;
	// png 为四通道图片
	// 是否需要翻转图片
	//stbi_set_flip_vertically_on_load(1);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureLocalBuffer = stbi_load(path.data(), &textureWidth, &textureHeight, &nrChannels, 0);
	if (textureLocalBuffer) {
		if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureLocalBuffer);
		else if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureLocalBuffer);
		else
			std::cout << "Texture isn't png/jpg" << std::endl;

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(textureLocalBuffer);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

void Texture::Bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}
