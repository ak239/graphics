#include "stdafx.h"
#include "Plane.h"
#include "Shader.h"

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, 1.0f,-1.0f, // triangle 1 : begin
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, -1.0f, // triangle 1 : end
     1.0f, 1.0f,-1.0f, // triangle 2 : begin
     1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f};

static const GLfloat g_uv_buffer_data[] = { 
         0.0f,  1.0f, 
         0.0f,  0.0f, 
         1.0f,  1.0f, 
         1.0f,  1.0f, 
         1.0f,  0.0f,
		 0.0f,  0.0f
};

void Plane::init()
{
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &m_uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}

void Plane::setShader(Shader* _shader)
{
	m_shader = _shader;
	if (m_shader)
	{
		m_MV   = m_shader->getUniform<glm::mat4>("MV");
		m_P    = m_shader->getUniform<glm::mat4>("P");
	}
}

void Plane::renderImpl()
{
	if (m_shader)
	{
		m_shader->use();

		m_MV.setValue(getCameraView() * getModel());
		m_P.setValue(getProjection());
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
