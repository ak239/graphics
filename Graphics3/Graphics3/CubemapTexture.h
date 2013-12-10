#pragma once

#include "Texture.h"

#include <string>
using std::string;

class CubemapTexture : public Texture
{
public:
	CubemapTexture(void);
	~CubemapTexture(void);

	// files: {prefix}negx.dds {prefix}negy.dds {prefix}negz.dds
	// files: {prefix}posx.dds {prefix}posy.dds {prefix}posz.dds
	bool loadFromFile(const string& prefix);

	void activeAndBind(GLenum TextureUnit);

private:
	GLuint m_textureID;
};

