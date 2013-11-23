#pragma once

class Camera
{
public:
	virtual glm::mat4 view();

	void setPos(   const glm::vec3& pos){    m_pos = pos; }
	glm::vec3 getPos() const{ return m_pos; }

	void setLookAt(const glm::vec3& lookAt){ m_lookAt = lookAt; }
	void setHead(  const glm::vec3& head){   m_head = head; }

	void move(const glm::vec3& delta){ m_pos += delta; }

	void moveByDir(GLfloat d);
	glm::vec3 getDir() const{ return glm::normalize(m_pos - m_lookAt); }

private:
	glm::vec3 m_pos;
	glm::vec3 m_lookAt;
	glm::vec3 m_head;
};

