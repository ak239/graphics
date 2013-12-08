#pragma once

#include "RenderObject.h"
#include "Shader.h"

#include <istream>
#include <map>

class StreamObject : public RenderObject
{
public:
	explicit StreamObject(GLContext _context);
	StreamObject(const StreamObject& object);
	~StreamObject(void);

	bool loadObject(std::istream& stream);

	void setShader(Shader* _shader);

private:
	void renderImpl();

	void clear();

	bool processVertex(       const std::vector<std::string>& data, int line);
	bool processVertexTexture(const std::vector<std::string>& data, int line);
	bool processVertexNormal( const std::vector<std::string>& data, int line);
	bool processVertexSpace(  const std::vector<std::string>& data, int line);
	bool processFace(         const std::vector<std::string>& data, int line);

	std::vector<GLfloat> m_vertexes;
	std::vector<GLfloat> m_vertexesTexture;
	std::vector<GLfloat> m_vertexesNormal;
	std::vector<GLfloat> m_fragmentNormal;
	std::vector<GLfloat> m_vertexesSpace;
	std::vector<std::vector<GLuint> > m_faces;
	std::vector<std::vector<GLuint> > m_indices;

	void fillBuffers();
	void decompressBuffers();

	GLuint m_vertexBuffer;
	GLuint m_uvBuffer;
	GLuint m_elementBuffer;
	GLuint m_normalBuffer;
	GLuint m_fragmentNormalBuffer;

	UniformWrapper<glm::mat4> m_MV;
	UniformWrapper<glm::mat4> m_P;
	UniformWrapper<glm::mat4> m_invP;
	UniformWrapper<GLuint>    m_polygonModeUniform;

	Shader*                   m_shader;
public:
	typedef bool (StreamObject::*Process)(const std::vector<std::string>& data, int line);
	typedef std::map<std::string, StreamObject::Process> ProcessorsMap;
	static const ProcessorsMap Processors;
	static ProcessorsMap InitProcessors();
};

