#pragma once

#include <vector>

#include "Object.h"
#include "Camera.h"
#include "RenderObject.h"

class Scene : public Object
{
public:
	Scene():m_camera(0){}
	Scene(GLContext _context):Object(_context), m_camera(0){}

	void reshape(int width, int height);
	void display();
	void addObject(RenderObject* object);
	void setCamera(Camera* camera);

private:
	std::vector<RenderObject*> m_objects;
	Camera*                    m_camera;
};

