#include "stdafx.h"
#include "StreamObject.h"
#include "Logger.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// i need decompress vertexes from array with indexes to real array - add polygon normal for each vertex.

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

const StreamObject::ProcessorsMap StreamObject::Processors(StreamObject::InitProcessors());

StreamObject::StreamObject(GLContext _context) : RenderObject(_context), m_vertexBuffer(0), m_uvBuffer(0), m_elementBuffer(0), m_normalBuffer(0), m_shader(0)
{
}

StreamObject::StreamObject(const StreamObject& object) : RenderObject(object.getContext()),
	m_vertexes(object.m_vertexes), m_vertexesTexture(object.m_vertexesTexture),
	m_vertexesNormal(object.m_vertexesNormal), m_vertexesSpace(object.m_vertexesSpace),
	m_faces(object.m_faces), m_indices(object.m_indices),
	m_shader(object.m_shader)
{
	if (object.m_vertexBuffer != 0)
		fillBuffers();
}

StreamObject::~StreamObject(void)
{
	glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_uvBuffer);
	glDeleteBuffers(1, &m_elementBuffer);
	glDeleteBuffers(1, &m_normalBuffer);
}

bool StreamObject::loadObject(std::istream& stream)
{
	GET_CONTEXT();
	clear();
	int lineNumber = 0;
	while (!stream.eof() && !stream.fail() && !stream.bad())
	{
		++lineNumber;
		std::string line;
		std::getline(stream, line);
		if (stream.eof() || stream.fail() || stream.bad()) break;

		std::vector<std::string> strs;
		boost::split(strs, line, boost::is_any_of("\t "));

		if (strs.empty() || line.empty() || line[0] == '#') continue;

		ProcessorsMap::const_iterator it = Processors.find(strs[0]);
		if (it == Processors.end())
		{
			LOG_DEBUG(std::string("unknown line prefix: " + strs[0]));
			continue;
		}

		if (!CALL_MEMBER_FN(*this, it->second)(strs, lineNumber))
			return false;
	}

	decompressBuffers();
	fillBuffers();

	OPENGL_CHECK_FOR_ERRORS();

	return true;
}

void StreamObject::setShader(Shader* _shader)
{
	m_shader = _shader;
	if (m_shader)
	{
		m_MV   = m_shader->getUniform<glm::mat4>("MV");
		m_P    = m_shader->getUniform<glm::mat4>("P");
		//m_invP = m_shader->getUniform<glm::mat4>("invP");

		//m_polygonModeUniform = m_shader->getUniform<GLuint>("polygonMode");
	}
}

void StreamObject::renderImpl()
{
	//m_polygonModeUniform.setValue(getPolygonMode() == GL_FILL ? 1 : 0);

	if (m_shader)
	{
		m_shader->use();

		m_MV.setValue(getCameraView() * getModel());
		m_P.setValue(getProjection());
		//m_invP.setValue(glm::inverse(getProjection()));
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_fragmentNormalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

	glDrawElements(GL_TRIANGLES, m_indices[0].size(), GL_UNSIGNED_INT, (void*)(0));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	OPENGL_CHECK_FOR_ERRORS();
}

void StreamObject::clear()
{
	m_vertexes.clear();
	m_vertexesTexture.clear();
	m_vertexesNormal.clear();
	m_vertexesSpace.clear();
	m_faces.clear();
}

bool StreamObject::processVertex(const std::vector<std::string>& data, int line)
{
	const std::string name = "vertex (v)";

	if (data.size() != 4 && data.size() < 5)
	{
		LOG_ERROR((boost::format("incorrect arguments count in %s line: %d") % name % line).str());
		return false;
	}

	try
	{
		GLfloat x = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat y = boost::lexical_cast<GLfloat>(data[2]);
		GLfloat z = boost::lexical_cast<GLfloat>(data[3]);
		GLfloat w = data.size() == 5 ? boost::lexical_cast<GLfloat>(data[4]) : 1.0f;

		m_vertexes.push_back(x);
		m_vertexes.push_back(y);
		m_vertexes.push_back(z);
		m_vertexes.push_back(w);
	}
	catch (boost::bad_lexical_cast const&)
	{
		LOG_ERROR((boost::format("bad argument value in %s line: %d") % name % line).str());
		return false;
	}
	return true;
}

bool StreamObject::processVertexTexture(const std::vector<std::string>& data, int line)
{
	const std::string name = "vertex texture (vt)";

	if (data.size() < 2 && data.size() > 4)
	{
		LOG_ERROR((boost::format("incorrect arguments count in %s line: %d") % name % line).str());
		return false;
	}

	try
	{
		GLfloat u = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat v = data.size() > 2 ? boost::lexical_cast<GLfloat>(data[2]) : 0.0f;
		GLfloat w = data.size() > 3 ? boost::lexical_cast<GLfloat>(data[3]) : 0.0f;

		m_vertexesTexture.push_back(u);
		m_vertexesTexture.push_back(v);
		m_vertexesTexture.push_back(w);
	}
	catch (boost::bad_lexical_cast const&)
	{
		LOG_ERROR((boost::format("bad argument value in %s line: %d") % name % line).str());
		return false;
	}
	return true;
}

bool StreamObject::processVertexNormal(const std::vector<std::string>& data, int line)
{
	const std::string name = "vertex normal (vn)";

	if (data.size() != 4)
	{
		LOG_ERROR((boost::format("incorrect arguments count in %s line: %d") % name % line).str());
		return false;
	}

	try
	{
		GLfloat x = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat y = boost::lexical_cast<GLfloat>(data[2]);
		GLfloat z = boost::lexical_cast<GLfloat>(data[3]);

		m_vertexesNormal.push_back(x);
		m_vertexesNormal.push_back(y);
		m_vertexesNormal.push_back(z);
	}
	catch (boost::bad_lexical_cast const&)
	{
		LOG_ERROR((boost::format("bad argument value in %s line: %d") % name % line).str());
		return false;
	}
	return true;
}

bool StreamObject::processVertexSpace(const std::vector<std::string>& data, int line)
{
	const std::string name = "vertex space (vp)";

	if (data.size() < 2 && data.size() > 4)
	{
		LOG_ERROR((boost::format("incorrect arguments count in %s line: %d") % name % line).str());
		return false;
	}

	try
	{
		GLfloat u = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat v = data.size() > 2 ? boost::lexical_cast<GLfloat>(data[2]) : 0.0f;
		GLfloat w = data.size() > 3 ? boost::lexical_cast<GLfloat>(data[3]) : 0.0f;

		m_vertexesSpace.push_back(u);
		m_vertexesSpace.push_back(v);
		m_vertexesSpace.push_back(w);
	}
	catch (boost::bad_lexical_cast const&)
	{
		LOG_ERROR((boost::format("bad argument value in %s line: %d") % name % line).str());
		return false;
	}
	return true;
}

bool StreamObject::processFace(const std::vector<std::string>& data, int line)
{
	const std::string name = "face (f)";

	if (data.size() < 4)
	{
		LOG_ERROR((boost::format("incorrect arguments count in %s line: %d") % name % line).str());
		return false;
	}

	// for each type of faces (triangles, quads, ...) - sepparate array
	m_faces.resize(data.size() - 3);
	m_indices.resize(data.size() - 3);

	std::vector<GLuint>& face   = m_faces[  data.size() - 4];
	std::vector<GLuint>& indice = m_indices[data.size() - 4];

	for (std::size_t i = 1; i < data.size(); ++i)
	{
		std::vector<std::string> strs;
		boost::split(strs, data[i], boost::is_any_of("/"));

		try
		{
			GLuint vertexIdx  = boost::lexical_cast<GLuint>(strs[0]);
			GLuint textureIdx = (strs.size() > 1 && !strs[1].empty()) ? boost::lexical_cast<GLuint>(strs[1]) : 0;
			GLuint normalIdx  = strs.size() > 2 ? boost::lexical_cast<GLuint>(strs[2]) : 0;

			indice.push_back(vertexIdx - 1);
			face.push_back(textureIdx);
			face.push_back(normalIdx);
		}
		catch (boost::bad_lexical_cast const&)
		{
			LOG_ERROR((boost::format("bad argument value in %s line: %d") % name % line).str());
			return false;
		}		
	}

	size_t first = indice[0] * 4;
	glm::vec3 p1(m_vertexes[first], m_vertexes[first + 1], m_vertexes[first + 2]);
	first = indice[1] * 4;
	glm::vec3 p2(m_vertexes[first], m_vertexes[first + 1], m_vertexes[first + 2]);
	first = indice[2] * 4;
	glm::vec3 p3(m_vertexes[first], m_vertexes[first + 1], m_vertexes[first + 2]);

	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;

	glm::vec3 n = glm::normalize(glm::cross(v1, v2));

	size_t maxIdx = std::max(indice[0], std::max(indice[1], indice[2]));
	if (maxIdx * 4 >= m_fragmentNormal.size())
		m_fragmentNormal.resize((maxIdx + 1) * 4);

	for (size_t i = 0; i < 3; ++i)
	{
		m_fragmentNormal[indice[i] * 4 + 0] = n.x;
		m_fragmentNormal[indice[i] * 4 + 1] = n.y;
		m_fragmentNormal[indice[i] * 4 + 2] = n.z;
	}

	return true;
}


void StreamObject::decompressBuffers()
{
}


void StreamObject::fillBuffers()
{
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexes.size() * sizeof(GLfloat), &m_vertexes[0], GL_STATIC_DRAW);
	m_vertexBuffer = vertexBuffer;

	if (!m_vertexesTexture.empty())
	{
		GLuint uvBuffer;
		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vertexesTexture.size() * sizeof(GLfloat), &m_vertexesTexture[0], GL_STATIC_DRAW);
		m_uvBuffer = uvBuffer;
	}

	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices[0].size() * sizeof(GLuint), &m_indices[0][0], GL_STATIC_DRAW);
	m_elementBuffer = elementBuffer;

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexesNormal.size() * sizeof(GLfloat), &m_vertexesNormal[0], GL_STATIC_DRAW);
	m_normalBuffer = normalBuffer;

	GLuint fragmentNormalBuffer = 0;
	glGenBuffers(1, &fragmentNormalBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fragmentNormalBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_fragmentNormal.size() * sizeof(GLfloat), &m_fragmentNormal[0], GL_STATIC_DRAW);
	m_fragmentNormalBuffer = fragmentNormalBuffer;

	OPENGL_CHECK_FOR_ERRORS();
}

StreamObject::ProcessorsMap StreamObject::InitProcessors()
{
	ProcessorsMap map;
	map["v"]  = &StreamObject::processVertex;
	map["vt"] = &StreamObject::processVertexTexture;
	map["vn"] = &StreamObject::processVertexNormal;
	map["vp"] = &StreamObject::processVertexSpace;
	map["f"]  = &StreamObject::processFace;
	return map;
}
