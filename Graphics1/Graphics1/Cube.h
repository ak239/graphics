#pragma once

#include "RenderObject.h"
#include "UniformWrapper.h"

class Shader;

class Cube : public RenderObject
{
public:
	void init();
	void setShader(Shader* _shader);

private:
	virtual void renderImpl();

	GLuint m_vertexBuffer;
	GLuint m_uvBuffer;

	UniformWrapper<glm::mat4> m_MV;
	UniformWrapper<glm::mat4> m_P;

	Shader*                   m_shader;
};

