#pragma once

#include <string>
using std::string;

class Texture
{
public:
	Texture(void);
	~Texture(void);

	virtual bool loadFromFile(const string& fileName, GLint textureUnit);

	GLuint textureID() const{ return m_textureID; }

	void active();
	virtual void activeAndBind();
	void setWrap(GLint wrapType);
	void setMinFilter(GLint filter);
	void setMagFilter(GLint filter);
protected:
	void setTextureID(GLuint _textureID){ m_textureID = _textureID; }
	void setTextureUnit(GLint _textureUnit){ m_textureUnit = _textureUnit; }

private:
	GLuint m_textureID;
	GLint  m_textureUnit;
};

