#include "GL.h"
#include "Cube.h"

#include "Shader.h"
#include "ArrayStaticBuffer.h"
#include "CubemapTexture.h"

static const GLfloat g_vertex_buffer_data[] = { 
	-1.0f, -1.0f, -1.0f, 
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,	
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,	
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

void Cube::init()
{
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void Cube::setShader(Shader* _shader)
{
	m_shader = _shader;
	if (m_shader)
	{
		m_shader->use();
		m_MV   = m_shader->getUniform<glm::mat4>("MV");
		m_P    = m_shader->getUniform<glm::mat4>("P");
	}
}

void Cube::setTexture(CubemapTexture* _texture)
{
	m_texture = _texture;
}

void Cube::renderImpl()
{
	if (m_shader)
	{
		m_shader->use();

		m_MV.setValue(getCameraView() * getModel() * glm::scale(vec3(5.0)) *(glm::rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f))));
		m_P.setValue(getProjection());
	}

	GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
	
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	
	m_texture->activeAndBind(GL_TEXTURE1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);

    glCullFace(OldCullFaceMode); 
    glDepthFunc(OldDepthFuncMode);
}
