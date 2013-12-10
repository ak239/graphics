#pragma once

#include "RenderObject.h"
#include "UniformWrapper.h"
#include "ArrayStaticBuffer.h"

class CubemapTexture;
class Shader;

class Cube : public RenderObject
{
public:
	Cube(GLContext context) : RenderObject(context){}

	void init();
	void setShader(Shader* _shader);
	void setTexture(CubemapTexture* _texture);

private:
	virtual void renderImpl();

	/*ArrayStaticBuffer<GLfloat> m_vertexBuffer;*/
	GLuint m_vertexBuffer;
	UniformWrapper<glm::mat4> m_MV;
	UniformWrapper<glm::mat4> m_P;

	Shader*                   m_shader;
	CubemapTexture*			  m_texture;
};

