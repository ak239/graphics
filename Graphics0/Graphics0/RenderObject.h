#pragma once

#include "Object.h"
#include <vector>

class RenderObject : public Object
{
public:
	RenderObject() : m_polygonMode(GL_FILL), m_factor(0.0f), m_units(0.0f){}
	RenderObject(GLContext _context):Object(_context), m_polygonMode(GL_FILL), m_factor(0.0f), m_units(0.0f){}

	void render();
	virtual void reshape(int width, int height){}

	void setProjectionView(const glm::mat4& projectionView);
	void setCameraPos(const glm::vec3& camPos);

	void setPosition(const glm::vec3& position);
	const glm::vec3& getPosition() const;

	virtual glm::mat4 getModel();

	void setCameraDir(const glm::vec3& cameraDir)  { m_cameraDir = cameraDir; }
	void setCameraView(const glm::mat4& cameraView){ m_cameraView = cameraView; }
	
	void setProjection(const glm::mat4& projection){ m_projection = projection; }
	void setPolygonMode(GLuint mode){ m_polygonMode = mode; }
	void setPolygonOffset(GLfloat factor, GLfloat units){ m_factor = factor; m_units = units; }

protected:
	const glm::mat4& getProjectionView() const;
	const glm::vec3& getCameraPos() const;
	const glm::vec3& getCameraDir() const{  return m_cameraDir; }
	const glm::mat4& getCameraView() const{ return m_cameraView; }
	const glm::mat4& getProjection() const{ return m_projection; }
	GLuint getPolygonMode() const{ return m_polygonMode; }
	GLfloat getFactor() const{ return m_factor; }
	GLfloat getUnits()  const{ return m_units;  }

private:
	virtual void renderImpl() = 0;

	glm::mat4 m_projectionView;
	glm::mat4 m_projection;
	glm::vec3 m_camPos;
	glm::vec3 m_position;
	glm::vec3 m_cameraDir;
	glm::mat4 m_cameraView;

	GLuint  m_polygonMode;
	GLfloat m_factor;
	GLfloat m_units;
};

