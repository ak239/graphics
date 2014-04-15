#include "geometric.h"

#include "glerror.h"

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

GLuint genCubeVertexBuffer(const glm::vec3& scale)
{
	GLuint vertexBuffer = 0;

	GLfloat buf[36 * 3];
	memcpy_s(buf, 36 * 3  * sizeof(GLfloat), g_vertex_buffer_data, 36 * 3 * sizeof(GLfloat));

	for (size_t i = 0; i < 36 * 3; ++i)
		buf[i] *= scale[i % 3];// == 0 ? scale.x : i % 3 == 1 ? scale.y : scale.z;

	GE(glGenBuffers(1, &vertexBuffer));
    GE(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GE(glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), buf, GL_STATIC_DRAW));
	return vertexBuffer;
}