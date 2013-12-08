#pragma once

#include <string>

class Texture
{
public:
	Texture(void);
	~Texture(void);

	bool loadFromFile(const char* fileName, GLint textureUnit);

	GLuint textureID() const{ return m_textureID; }

	void active();
	void activeAndBind();
	void setWrap(GLint wrapType);
	void setMinFilter(GLint filter);
	void setMagFilter(GLint filter);
private:
	GLuint m_textureID;
	GLint  m_textureUnit;
};

