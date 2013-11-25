#include "stdafx.h"

#include "Sphere.h"
#include "Shader.h"

void Sphere::subdivide(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, std::vector<glm::vec3> &sphere_points, const unsigned int depth)
{
	if(depth == 0) {
		sphere_points.push_back(v1);
		sphere_points.push_back(v2);
		sphere_points.push_back(v3);
        return;
    }
	const glm::vec3 v12 = glm::normalize(v1 + v2);
    const glm::vec3 v23 = glm::normalize(v2 + v3);
    const glm::vec3 v31 = glm::normalize(v3 + v1);
    subdivide(v1,  v12, v31, sphere_points, depth - 1);
    subdivide(v2,  v23, v12, sphere_points, depth - 1);
    subdivide(v3,  v31, v23, sphere_points, depth - 1);
    subdivide(v12, v23, v31, sphere_points, depth - 1);
}

void Sphere::init(const unsigned int depth)
{
    const double X = 0.525731112119133606;
    const double Z = 0.850650808352039932;
    const glm::vec3 vdata[12] = {
        glm::vec3(-X, 0.0, Z),  glm::vec3( X, 0.0, Z ),  glm::vec3( -X, 0.0, -Z ), glm::vec3( X, 0.0, -Z ),
        glm::vec3( 0.0, Z, X ), glm::vec3( 0.0, Z, -X ), glm::vec3( 0.0, -Z, X ),  glm::vec3( 0.0, -Z, -X ),
        glm::vec3( Z, X, 0.0 ), glm::vec3( -Z, X, 0.0 ), glm::vec3( Z, -X, 0.0 ),  glm::vec3( -Z, -X, 0.0 )
    };
    int tindices[20][3] = {
        {0, 4, 1}, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 }, { 4, 8, 1 },
        { 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5, 2, 3 }, { 2, 7, 3 },
        { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
        { 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 }
    };
    for(int i = 0; i < 20; i++)
        subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], m_vexrtexBufferData, depth);

	for (size_t i = 0; i < m_vexrtexBufferData.size(); i++)
	{
		const glm::vec3& point = m_vexrtexBufferData[i];
		GLfloat r     = glm::length(point);

		GLfloat theta = 0.0f;
		theta = glm::acos(point.z / r);
		GLfloat phi = 0.0f;
		phi   = glm::atan2(point.y, point.x);	

		m_uvBufferData.push_back(phi / 2.0f / glm::pi<GLfloat>());
		m_uvBufferData.push_back(1.0f - theta / glm::pi<GLfloat>());
	}

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vexrtexBufferData.size() * sizeof(glm::vec3), m_vexrtexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_uvBufferData.size() * sizeof(GLfloat), m_uvBufferData.data(), GL_STATIC_DRAW);
}

void Sphere::setShader(Shader* _shader)
{
	m_shader = _shader;
	if (m_shader)
	{
		m_MV   = m_shader->getUniform<glm::mat4>("MV");
		m_P    = m_shader->getUniform<glm::mat4>("P");
	}
}

void Sphere::renderImpl()
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

	glDrawArrays(GL_TRIANGLES, 0, m_vexrtexBufferData.size());

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}