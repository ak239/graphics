#include "Texture.h"

#include <ext.hpp>

GLuint load3DTexture(const vector<unsigned char>& data, GLint width, GLint height, GLint depth)
{
	GLuint texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_3D, texID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	return texID;
}

vector<unsigned char> genSphere(GLint width)
{
	vector<unsigned char> data(width * width * width * 4, 0);

	for (size_t i = 0; i < width; ++i)
		for (size_t j = 0; j < width; ++j)
			for (size_t k = 0; k < width; ++k)
			{
				GLfloat w = static_cast<GLfloat>(width);
				GLfloat x = static_cast<GLfloat>(i) - (w / 2);
				GLfloat y = static_cast<GLfloat>(j) - (w / 2);
				GLfloat z = static_cast<GLfloat>(k) - (w / 2);

				GLfloat r = glm::sqrt(x * x + y * y + z * z);

				data[i * width * width + j * width + k] = (GLubyte)(r);
			}

	return data;
}
