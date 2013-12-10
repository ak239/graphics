#pragma once

class Camera
{
public:
	virtual mat4 view();

	void setPos(const vec3& pos){ m_pos = pos; }
	vec3 getPos() const{ return m_pos; }

	void setLookAt(const vec3& lookAt){ m_lookAt = lookAt; }
	void setHead(  const vec3& head){   m_head = head; }

	void move(const vec3& delta){ m_pos += delta; }

	void moveByDir(GLfloat d);
	vec3 getDir() const{ return glm::normalize(m_pos - m_lookAt); }
private:
	vec3 m_pos;
	vec3 m_lookAt;
	vec3 m_head;
};

