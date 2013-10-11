#pragma once

#include <vector>
#include "RenderObject.h"

template<typename MoveFunc>
class Mover
{
public:
	Mover(){}
	~Mover(){};

	void addObject(RenderObject* obj);
	void setFunc(MoveFunc func);
	void idleFunc();

private:
	std::vector<std::pair<RenderObject*, glm::vec3> > m_objects;

	MoveFunc m_Func;
};

template<typename MoveFunc>
void Mover<MoveFunc>::addObject(RenderObject* obj)
{
	m_objects.push_back(std::make_pair(obj, obj->getPosition()));
}

template<typename MoveFunc>
void Mover<MoveFunc>::setFunc(MoveFunc func)
{
	m_Func = func;
}

template<typename MoveFunc>
void Mover<MoveFunc>::idleFunc()
{
	GLuint time = glutGet(GLUT_ELAPSED_TIME);

	for (std::size_t i = 0; i < m_objects.size(); ++i)
		m_objects[i].first->setPosition(m_objects[i].second + m_Func(static_cast<GLfloat>(time)));
}
