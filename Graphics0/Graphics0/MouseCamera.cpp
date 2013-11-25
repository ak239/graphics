#include "stdafx.h"
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
	m_dist = dist;
	invalidate();
}

void MouseCamera::invalidate()
{
	//setLookAt(glm::vec3(0.0f));
	//setHead(glm::vec3(0.0f, 1.0f, 0.0f));
	//setPos(glm::vec3(m_dist * -glm::cos(m_heading) * glm::sin(m_pitch), 
	//				 m_dist * -glm::sin(m_pitch),
	//				 -m_dist * glm::cos(m_heading) * glm::cos(m_pitch)));

}

glm::mat4 MouseCamera::view()
{
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -m_dist),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
		);	
	return View * glm::toMat4(m_rot);
}