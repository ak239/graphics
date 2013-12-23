#include "GL.h"
#include "Texture.h"

#include "nvImage.h"

Texture::Texture(void) : m_textureID(-1)
{
}


Texture::~Texture(void)
{
	if (m_textureID != -1)
		glDeleteTextures(1, &m_textureID);
}

bool Texture::loadFromFile(const string& fileName, GLint textureUnit)
{
	nv::Image img;
	if (!img.loadImageFromFile(fileName.c_str()))
		return false;

	GLuint textureID;
    glGenTextures(1, &textureID);
	glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D( GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_textureID   = textureID;
	m_textureUnit = textureUnit;

	return true;
}

void Texture::active()
{
	glActiveTexture(m_textureUnit);
}

void Texture::activeAndBind()
{
	glActiveTexture(m_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::setWrap(GLint wrapType)
{
	active();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
}

void Texture::setMinFilter(GLint filter)
{
	active();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
}

void Texture::setMagFilter(GLint filter)
{
	active();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
}

Texture1D::Texture1D(void) : m_textureID(-1){}

Texture1D::~Texture1D(void){}

bool Texture1D::loadFromFile(const string& fileName)
{
	nv::Image img;
	if (!img.loadImageFromFile(fileName.c_str()))
		return false;

	GLuint textureID;
    glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, textureID);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage1D( GL_TEXTURE_1D, 0, img.getInternalFormat(), img.getWidth(), 0, img.getFormat(), img.getType(), img.getLevel(0));
	glGenerateMipmap(GL_TEXTURE_1D);

	return true;
}

void Texture1D::active(GLint textureUnit)
{
	glActiveTexture(textureUnit);
}

void Texture1D::bind(GLint textureUnit)
{
	glBindTexture(GL_TEXTURE_1D, m_textureID);
}
