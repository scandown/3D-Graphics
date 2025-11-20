#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.h"

unsigned int texture_setup(jmp_buf error, GLenum format, char *name) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width;
	int height;
	int nrChannels;
	unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);

	if (!data) {
		printf("Couldn't load file: %s\n", name);
		longjmp(error, 1);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(data);
	
	return texture;
}
