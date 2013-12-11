#pragma once

#include <vector>
using std::vector;

#include "stdafx.h"

template<typename VertexType>
class ArrayStaticBuffer
{
public:
	ArrayStaticBuffer();
	~ArrayStaticBuffer(void);
	
	// set buffer with name - name_ to current GL_ARRAY_BUFFER
	void bind() const;
	// store new data in buffer
	void bufferData(const vector<VertexType>& _data);

	GLuint count() const{ return count_; }
private:
	GLuint name_;
	GLuint count_;
};


template<typename VertexType>
ArrayStaticBuffer<VertexType>::ArrayStaticBuffer() : 
	name_(0), count_(0)
{
	glGenBuffers(1, &name_);
}


template<typename VertexType>
ArrayStaticBuffer<VertexType>::~ArrayStaticBuffer(void)
{
	glDeleteBuffers(1, &name_);
}


template<typename VertexType>
void ArrayStaticBuffer<VertexType>::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, name_);
}


template<typename VertexType>
void ArrayStaticBuffer<VertexType>::bufferData(const vector<VertexType>& _data)
{
	bind();
	count_ = _data.size();
	glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(VertexType), _data.data(), GL_STATIC_DRAW);
}
