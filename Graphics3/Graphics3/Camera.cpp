#include "GL.h"
#include "Camera.h"

mat4 Camera::view()
{
	mat4 View = glm::lookAt(
		m_pos,
		m_lookAt,
		m_head
	);	
	return View;
}

void Camera::moveByDir(GLfloat d)
{
	vec3 dir = getDir();
	dir *= d;
	m_pos += dir;
}