#pragma once

#include "Camera.h"

class MouseCamera : public Camera
{
public:
	MouseCamera():m_heading(0.0f), m_pitch(0.0f), m_dist(20.0f), m_speed(3.0f){}

	void setAngles(GLfloat heading, GLfloat pitch);
	void rotate(GLfloat heading, GLfloat pitch);
	GLfloat getHeading() const{ return m_heading; }
	GLfloat getPitch()   const{ return m_pitch;   }

	void setDist(GLfloat dist);
	GLfloat getDist() const{ return m_dist; }

	glm::mat4 view();

private:
	void invalidate();

	GLfloat m_heading;
	GLfloat m_pitch;
	GLfloat m_dist;

	GLfloat m_speed;

	glm::quat m_rot;
};

