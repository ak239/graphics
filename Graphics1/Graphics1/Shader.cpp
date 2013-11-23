#include "stdafx.h"
#include "Shader.h"
#include <boost\format.hpp>
#include <fstream>

#include "GLContext.h"
#include "Common.h"

Shader::~Shader(void)
{
	GET_CONTEXT();

	unUse();
    glDeleteProgram(m_program);
}

bool Shader::loadFromFile(const char* fileName, GLuint type)
{
	GET_CONTEXT();

	GLuint   program;
	std::string shaderName;

	if ((program = glCreateProgram()) == 0){
		LOG_ERROR(boost::str(boost::format("Creating shader program fail (%d)\n") % glGetError()));
		return false;
	}

	// если необходимо создать вершинный шейдер
	if (type & Vertex){
		// им€ вершинного шейдера
		shaderName    = boost::str(boost::format("%s.vs") % fileName);
		if (!loadShader(shaderName, GL_VERTEX_SHADER, program)){
			glDeleteProgram(program);
			return false;		
		}
	}

	// если необходимо создать фрагментный шейдер
	if (type & Fragment)
	{
		shaderName    = boost::str(boost::format("%s.fs") % fileName);
		if (!loadShader(shaderName, GL_FRAGMENT_SHADER, program)){
			glDeleteProgram(program);
			return false;		
		}
	}
	OPENGL_CHECK_FOR_ERRORS();

	m_program = program;
	return true;
}

GLint Shader::getShaderStatus(GLuint shader, GLenum param)
{
	GET_CONTEXT();

    GLint status = 0, length = 0;
    GLchar buffer[4096 * 4];

    glGetShaderiv(shader, param, &status);

    if (status != GL_TRUE)
    {
        glGetShaderInfoLog(shader, 4096 * 4, &length, buffer);
		buffer[length] = '\0';
		LOG_ERROR(boost::str(boost::format("Shader: %s\n") % (const char*)buffer));
    }

    OPENGL_CHECK_FOR_ERRORS();

    return status;
}

GLint Shader::shaderProgramStatus(GLuint program, GLenum param)
{
	GET_CONTEXT();

    GLint status = 0, length = 0;
    GLchar buffer[1024];

	glGetProgramiv(program, param, &status);

    if (status != GL_TRUE)
    {
		glGetProgramInfoLog(program, 1024, &length, buffer);
		buffer[length] = '\0';
		LOG_ERROR(boost::str(boost::format("Shader: %s\n") % (const char*)buffer));
    }

    OPENGL_CHECK_FOR_ERRORS();

    return status;
}

bool Shader::loadShader(const std::string& fileName, GLuint type, GLuint program)
{
	GET_CONTEXT();

	GLint shader = 0;
	// создадим вершинный шейдер
	if ((shader = glCreateShader(type)) == 0){
		LOG_ERROR(boost::str(boost::format("Creating vertex shader fail (%d)\n") % glGetError()));
		return false;
	}

	GLint sourceLength = 0;
	char* shaderSource = Common::GetFileContent(fileName, &sourceLength);
	if (!shaderSource){
		glDeleteShader(shader);
		LOG_ERROR(boost::str(boost::format("Creating shader program fail: can't get file content - %s") % fileName));
		return false;
	}

	// зададим шейдеру исходный код и скомпилируем его
	glShaderSource(shader, 1, (const GLchar**)&shaderSource, &sourceLength);
	glCompileShader(shader);
	delete [] shaderSource;

	// проверим статус компил€ции шейдера
	if (getShaderStatus(shader, GL_COMPILE_STATUS) != GL_TRUE)
	{
		glDeleteShader(shader);
		return false;
	}

	// присоеденим загруженные шейдеры к шейдерной программе
	glAttachShader(program, shader);

	// шейдер нам больше не нужен, пометим его на удаление
	// он будет удален вместе с шейдерной программой
	glDeleteShader(shader);
	return true;
}

void Shader::use()
{
	GET_CONTEXT();
	glUseProgram(m_program);
}

void Shader::unUse()
{
	GET_CONTEXT();
	glUseProgram(0);
}

bool Shader::isValid()
{
	GET_CONTEXT();
	glValidateProgram(m_program);
	return (getShaderStatus(m_program, GL_VALIDATE_STATUS) == GL_TRUE);
}

bool Shader::link()
{
	GET_CONTEXT();
	glLinkProgram(m_program);
    return (shaderProgramStatus(m_program, GL_LINK_STATUS) == GL_TRUE);
}