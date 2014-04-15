#include "texture.h"

#include <cstring>
#include <cstdio>

#include "nvImage.h"
#include "glerror.h"

bool loadDDSImage(nv::Image* img, const char* folder, const char* name);

#define ARRAY_SIZE(a)                               \
  ((sizeof(a) / sizeof(*(a))) /                     \
  static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

const GLenum CUBEMAP_TYPES[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

const char* CUBEMAP_FILES[] = {
	"posx.dds",
	"negx.dds",
	"posy.dds",
	"negy.dds",
	"posz.dds",
	"negz.dds"
};

GLuint loadCubemap(const char* folder)
{
	GLuint textureID;
    GE(glGenTextures(1, &textureID));
    GE(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));

	bool isOk = true;
	for (int i = 0; i < ARRAY_SIZE(CUBEMAP_TYPES) && isOk; ++i)
	{
		nv::Image img;
		isOk = loadDDSImage(&img, folder, CUBEMAP_FILES[i]);
		if (isOk)
		{
			GLvoid* data = (GLvoid*)img.getLevel(0);		
			GE(glCompressedTexImage2D(CUBEMAP_TYPES[i], 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getImageSize(), data));
		}
	}
	if (!isOk)
	{
		glDeleteTextures(1, &textureID);
		return 0;
	}
	GE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GE(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));    
	GE(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	
	return textureID;
}

bool loadDDSImage(nv::Image* img, const char* folder, const char* name)
{
	char fileName[MAX_PATH];

	strcpy_s(fileName, MAX_PATH, folder);
	strcat_s(fileName, MAX_PATH, "/");
	strcat_s(fileName, MAX_PATH, name);

	bool isOk = img->loadImageFromFile(fileName);
	if (isOk)
		fprintf(stdout, "cubemap loaded from: %s\n", fileName);
	else
		fprintf(stderr, "can't load cubemap from: %s\n", fileName);
	return isOk;
}
