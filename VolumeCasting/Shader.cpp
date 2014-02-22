#include "Shader.h"

#include <string>
using std::string;
#include <istream>
using std::istream;
#include <sstream>
using std::stringstream;
#include <iterator>
using std::istreambuf_iterator;
#include <fstream>
using std::fstream;
#include <cstdio>
#include <vector>
using std::vector;

string loadContent(const char *file)
{
	fstream f(file, std::ios::in);
	if (!f.is_open())
	{
		fprintf(stderr, "Can't open file: %s\n", file);
		return string();
	}
	return string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
}

GLuint compileShader(const string& content, GLuint type)
{
	GLuint shaderID = glCreateShader(type);
	const char *textPointer = content.c_str();
	glShaderSource(shaderID, 1, &textPointer, nullptr);

	GLint result = GL_FALSE;
	GLint infoLogLen = 0;

	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS,  &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);

	vector<char> errorMessage(std::max(infoLogLen, 1));
	glGetShaderInfoLog(shaderID, infoLogLen, nullptr, &errorMessage[0]);
	if (result != GL_TRUE)
		fprintf(stderr, "Shader Error: %s\n", &errorMessage[0]);
    
	return shaderID;
}

GLuint loadShaders(const char *vertexFile, const char *fragmentFile)
{
	string vertexContent = loadContent(vertexFile);
	string fragmentContent = loadContent(fragmentFile);
	if (vertexContent.empty() || fragmentContent.empty())
		return 0;

	GLuint vertexShaderID = compileShader(vertexContent, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = compileShader(fragmentContent, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, fragmentShaderID);
	glAttachShader(program, vertexShaderID);
	glLinkProgram(program);

    GLint result = GL_FALSE;
	GLint infoLogLen = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
    std::vector<char> errorMessage(std::max(infoLogLen, 1));
	glGetProgramInfoLog(program, infoLogLen, nullptr, &errorMessage[0]);
	if (result != GL_TRUE)
		fprintf(stderr, "Program Error: %s\n", &errorMessage[0]);
 
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	fprintf(stderr, "1: %d\n", result == GL_TRUE);

	glGetProgramiv(program, GL_ATTACHED_SHADERS, &result);
	fprintf(stderr, "2: %d\n", result);	

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
 
	return program;
}
