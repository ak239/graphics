#include "stdafx.h"
#include "ObjectFromObj.h"

#include "Shader.h"

ObjectFromObj::ObjectFromObj(GLContext _context) : 
	RenderObject(_context),
	vertexesBuffer_(-1), indicesBuffer_(-1), texturesBuffer_(-1), 
	normalsBuffer_(-1),  fragNormalsBuffer_(-1), indicesSize_(-1)
{
}


ObjectFromObj::~ObjectFromObj(void)
{
}

void ObjectFromObj::setData(Obj* data)
{
	vertexesBuffer_ = loadBuffer(data->vertexes());
	vertexesSize_   = data->vertexes().size();
	indicesBuffer_  = loadBuffer(data->vertexesIndices());
	indicesSize_    = data->vertexesIndices().size();
}

void ObjectFromObj::setData(DecompressObj* data)
{
	vertexesBuffer_ = loadBuffer(data->decompressVertexes());
	vertexesSize_   = data->decompressVertexes().size();
	if (!data->decompressTextures().empty())
		texturesBuffer_ = loadBuffer(data->decompressTextures());
	if (!data->normals().empty())
		normalsBuffer_ = loadBuffer(data->decompressNormals());
}

void ObjectFromObj::setData(ObjWithFragNormals* data)
{
	vertexesBuffer_ = loadBuffer(data->decompressVertexes());
	vertexesSize_   = data->decompressVertexes().size();
	if (!data->decompressTextures().empty())
		texturesBuffer_ = loadBuffer(data->decompressTextures());
	if (!data->decompressNormals().empty())
		normalsBuffer_ = loadBuffer(data->decompressNormals());
	if (!data->fragNormals().empty())
		fragNormalsBuffer_ = loadBuffer(data->fragNormals());
}

void ObjectFromObj::setShader(Shader* _shader)
{
	shader_ = _shader;
	if (shader_)
	{
		MV_ = shader_->getUniform<mat4>("MV");
		P_  = shader_->getUniform<mat4>("P");
	}
}

void ObjectFromObj::renderImpl()
{
	if (shader_)
	{
		shader_->use();

		MV_.setValue(getCameraView() * getModel());
		P_.setValue(getProjection());
	}

	int lastBuf = -1;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesBuffer_);
	glVertexAttribPointer(++lastBuf, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	if (indicesBuffer_ == -1)
	{
		if (texturesBuffer_ != -1)
		{
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, texturesBuffer_);
			glVertexAttribPointer(++lastBuf, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		}

		if (normalsBuffer_ != -1)
		{
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer_);
			glVertexAttribPointer(++lastBuf, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		}

		if (fragNormalsBuffer_ != -1)
		{
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, fragNormalsBuffer_);
			glVertexAttribPointer(++lastBuf, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		}

		glDrawArrays(GL_TRIANGLES, 0, vertexesSize_);
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer_);
		glDrawElements(GL_TRIANGLES, indicesSize_, GL_UNSIGNED_INT, (void*)(0));
	}

	for (int i = 0; i <= lastBuf; ++i)
		glDisableVertexAttribArray(i);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
