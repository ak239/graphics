#pragma once

#include "RenderObject.h"

#include "Obj.h"
#include "DecompressObj.h"
#include "ObjWithFragNormals.h"
#include "UniformWrapper.h"

using glm::mat4;

class Shader;

class ObjectFromObj : public RenderObject
{
public:
	explicit ObjectFromObj(GLContext _context);
	~ObjectFromObj(void);

	// simple object, only vertexes
	void setData(Obj*                data);
	// common object with vertexes, normals and textures
	void setData(DecompressObj*      data);
	// common object with added per fragment normals
	void setData(ObjWithFragNormals* data);

	void setShader(Shader* _shader);
private:
	void renderImpl();

	GLuint vertexesBuffer_;
	GLuint vertexesSize_;

	GLuint indicesBuffer_;
	GLuint indicesSize_;

	GLuint texturesBuffer_;
	GLuint normalsBuffer_;

	GLuint fragNormalsBuffer_;

	Shader* shader_;
	UniformWrapper<mat4> M_;
	UniformWrapper<mat4> V_;
	UniformWrapper<mat4> P_;
};

template<typename T>
GLuint loadBuffer(const vector<T>& data)
{
	GLuint tmp = -1;
	glGenBuffers(1, &tmp);
	glBindBuffer(GL_ARRAY_BUFFER, tmp);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
	return tmp;
}