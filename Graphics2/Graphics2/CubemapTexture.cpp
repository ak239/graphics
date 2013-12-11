#include "stdafx.h"
#include "CubemapTexture.h"

#include <vector>
#include "nvImage.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
	
bool CubemapTexture::loadFromFile(const string& prefix)
{
	using std::vector;
	vector<string> files(6);
	files[0] = prefix + "posx.dds";
	files[1] = prefix + "negx.dds";
	files[2] = prefix + "posy.dds";
	files[3] = prefix + "negy.dds";
	files[4] = prefix + "posz.dds";
	files[5] = prefix + "negz.dds";

	GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(types) ; i++)
	{
		nv::Image fImage;
		fImage.loadImageFromFile(files[i].c_str());

		unsigned char* data = (unsigned char*)fImage.getLevel(0);
		glTexImage2D(types[i], 0, fImage.getInternalFormat(), fImage.getWidth(), fImage.getHeight(), 0, fImage.getFormat(), fImage.getType(), data);
	}   

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);    
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	m_textureID = (textureID);

	return true;
}

void CubemapTexture::activeAndBind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}
