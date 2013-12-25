#pragma once

#include "GL.h"
#include "ArrayStaticBuffer.h"

template<typename VertexType>
class VertexAttribArray
{
public:
	VertexAttribArray(const ArrayStaticBuffer<VertexType>& _buffer, GLuint _index);
	~VertexAttribArray();

private:
	void setPointer(const ArrayStaticBuffer<VertexType>& _buffer);

	GLuint index_;
};


template<typename VertexType>
VertexAttribArray<VertexType>::VertexAttribArray(const ArrayStaticBuffer<VertexType>& _buffer, GLuint _index):
	index_(_index)
{
	glEnableVertexAttribArray(index_);
	_buffer.bind();
	setPointer(_buffer);
}


template<typename VertexType>
VertexAttribArray<VertexType>::~VertexAttribArray()
{
	glDisableVertexAttribArray(index_);
}

template<>
inline void VertexAttribArray<vec2>::setPointer(const ArrayStaticBuffer<vec2>& _buffer)
{
	glVertexAttribPointer(index_, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

template<>
inline void VertexAttribArray<vec3>::setPointer(const ArrayStaticBuffer<vec3>& _buffer)
{
	glVertexAttribPointer(index_, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

template<>
inline void VertexAttribArray<GLfloat>::setPointer(const ArrayStaticBuffer<GLfloat>& _buffer)
{
	glVertexAttribPointer(index_, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
}
