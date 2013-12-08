#pragma once

#include <vector>

#include "RenderObject.h"
#include "UniformWrapper.h"

class Shader;

class Sphere : public RenderObject
{
public:
	void init(const unsigned int depth);
	void setShader(Shader* _shader);

private:
	virtual void renderImpl();

	GLuint m_vertexBuffer;
	GLuint m_uvBuffer;

	UniformWrapper<glm::mat4> m_MV;
	UniformWrapper<glm::mat4> m_P;

	Shader*                   m_shader;

	std::vector<glm::vec3> m_vexrtexBufferData;
	std::vector<GLfloat> m_uvBufferData;

	void subdivide(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, std::vector<glm::vec3> &sphere_points, const unsigned int depth);
};

