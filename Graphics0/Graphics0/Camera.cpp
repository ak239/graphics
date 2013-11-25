#include "stdafx.h"
#include "Camera.h"

glm::mat4 Camera::view()
{
	glm::mat4 View = glm::lookAt(
		m_pos,
		m_lookAt,
		m_head
	);	
	return View;
}

void Camera::moveByDir(GLfloat d)
{
	glm::vec3 dir = getDir();
	dir *= d;
	m_pos += dir;
}