#include "GL.h"
#include "MouseCamera.h"

#include <glm/gtx/quaternion.hpp>

void MouseCamera::setAngles(GLfloat heading, GLfloat pitch)
{
	m_heading = heading;
	m_pitch   = pitch;
	invalidate();
}

void MouseCamera::rotate(GLfloat heading, GLfloat pitch)
{
	glm::quat quat(glm::vec3(-pitch, -heading, 0.0f));
	m_rot = quat * m_rot;
}

void MouseCamera::setDist(GLfloat dist)
{
	m_dist = abs(dist) > 3.0f ? 3.0f : dist;
	invalidate();
}

void MouseCamera::invalidate()
{
}

mat4 MouseCamera::view()
{
	mat4 View = glm::lookAt(
		vec3(0.0f, 0.0f, -m_dist),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
		);	
	return View * glm::toMat4(m_rot);
}