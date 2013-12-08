#pragma once

#include "stdafx.h"
#include "Object.h"

template<class T>
class UniformWrapper : public Object
{
public:
	UniformWrapper() : location(-1){}
	UniformWrapper(GLuint _location) : 
		Object(), location(_location){}

	UniformWrapper(GLContext _context, GLuint _location) : 
		Object(_context), location(_location){}

	const T& getValue() const{ return val; }

	void setValue(const T& _val){
		GLContextGetter get(getContext());
		val = _val;
		syncValue();
	}

private:
	void syncValue();

	T      val;
	GLuint location;
};

template<>
inline void UniformWrapper<glm::mat4>::syncValue(){
	glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]);
}

template<>
inline void UniformWrapper<GLuint>::syncValue(){
	glUniform1i(location, val);
}

template<>
inline void UniformWrapper<glm::vec2>::syncValue(){
	glUniform2f(location, val.x, val.y);
}

template<>
inline void UniformWrapper<glm::vec3>::syncValue(){
	glUniform3f(location, val.x, val.y, val.z);
}

template<>
inline void UniformWrapper<glm::vec4>::syncValue(){
	glUniform4f(location, val.x, val.y, val.z, val.w);
}

template<>
inline void UniformWrapper<GLfloat>::syncValue(){
	glUniform1f(location, val);
}

template<>
inline void UniformWrapper<glm::ivec2>::syncValue(){
	glUniform2i(location, val.x, val.y);
}

template<>
inline void UniformWrapper<GLfloat[7]>::syncValue(){
	glUniform1fv(location, sizeof(val) / sizeof(GLfloat), &val[0]);
}

template<>
inline void UniformWrapper<GLfloat[7]>::setValue(const GLfloat (&_val)[7]){
	GLContextGetter get(getContext());
	for (int i = 0; i < 7; ++i) val[i] = _val[i];
	syncValue();
}
