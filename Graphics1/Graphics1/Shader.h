#pragma once

#include "Object.h"
#include "UniformWrapper.h"

class Shader : public Object
{
public:
	Shader(GLContext _context):
		Object(_context){}
	~Shader(void);

	enum Type{
		Vertex   = 1 << 1,
		Fragment = 1 << 2
	};
	bool loadFromFile(const char* fileName, GLuint type);

	void use();
	void unUse();

	bool isValid();
	bool link();

	template<class T>
	UniformWrapper<T> getUniform(const char* name);

private:
	GLint getShaderStatus(GLuint shader, GLenum param);
	GLint shaderProgramStatus(GLuint program, GLenum param);
	bool  loadShader(const std::string& fileName, GLuint type, GLuint program);

	GLint m_program;
};

template<class T>
UniformWrapper<T> Shader::getUniform(const char* name)
{
	use();
	GLint loc = glGetUniformLocation(m_program, name);
	if (loc == -1) LOG_ERROR(boost::str(boost::format("Uniform not found: %s") % name));
	return UniformWrapper<T>(getContext(), loc);
}
