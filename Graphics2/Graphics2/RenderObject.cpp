#include "stdafx.h"
#include "RenderObject.h"

void RenderObject::render()
{
	if (m_isVisible)
	{
		GET_CONTEXT();
		glPolygonMode(GL_FRONT_AND_BACK, getPolygonMode());

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(getFactor(), getUnits());

		renderImpl();

		glDisable(GL_POLYGON_OFFSET_FILL);
	}
}

void RenderObject::setProjectionView(const glm::mat4& projectionView)
{
	m_projectionView = projectionView;
}

void RenderObject::setCameraPos(const glm::vec3& camPos)
{
	m_camPos = camPos;
}

const glm::mat4& RenderObject::getProjectionView() const
{
	return m_projectionView;
}

const glm::vec3& RenderObject::getCameraPos() const
{
	return m_camPos;
}

void RenderObject::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}
	
const glm::vec3& RenderObject::getScale() const
{
	return m_scale;
}

void RenderObject::setPosition(const glm::vec3& position)
{
	m_position = position;
}

const glm::vec3& RenderObject::getPosition() const
{
	return m_position;
}

glm::mat4 RenderObject::getModel()
{
	return glm::translate(m_position) * glm::scale(m_scale) * glm::mat4(1.0f);
}
