#pragma once

#include "GL.h"
#include "Object.h"
#include <vector>

class RenderObject : public Object
{
public:
	RenderObject() : m_polygonMode(GL_FILL), m_factor(0.0f), m_units(0.0f), m_isVisible(true){}
	RenderObject(GLContext _context):Object(_context), m_polygonMode(GL_FILL), m_factor(0.0f), m_units(0.0f), m_isVisible(true){}

	void render();
	virtual void reshape(int width, int height){}

	void setProjectionView(const mat4& projectionView);
	void setCameraPos(const vec3& camPos);

	void setPosition(const vec3& position);
	const vec3& getPosition() const;

	virtual mat4 getModel();

	void setCameraDir(const vec3& cameraDir)  { m_cameraDir = cameraDir; }
	void setCameraView(const mat4& cameraView){ m_cameraView = cameraView; }
	
	void setProjection(const mat4& projection){ m_projection = projection; }
	void setPolygonMode(GLuint mode){ m_polygonMode = mode; }
	void setPolygonOffset(GLfloat factor, GLfloat units){ m_factor = factor; m_units = units; }

	void setVisible(bool isVisible){ m_isVisible = isVisible; }

protected:
	const mat4& getProjectionView() const;
	const vec3& getCameraPos() const;
	const vec3& getCameraDir() const{  return m_cameraDir; }
	const mat4& getCameraView() const{ return m_cameraView; }
	const mat4& getProjection() const{ return m_projection; }
	GLuint getPolygonMode() const{ return m_polygonMode; }
	GLfloat getFactor() const{ return m_factor; }
	GLfloat getUnits()  const{ return m_units;  }

private:
	virtual void renderImpl() = 0;

	bool m_isVisible;

	mat4 m_projectionView;
	mat4 m_projection;
	vec3 m_camPos;
	vec3 m_position;
	vec3 m_cameraDir;
	mat4 m_cameraView;

	GLuint  m_polygonMode;
	GLfloat m_factor;
	GLfloat m_units;
};

