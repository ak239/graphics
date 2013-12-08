#include "stdafx.h"

#include "Obj.h"
#include "Logger.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

const Obj::ProcessorsMap Obj::Processors(Obj::InitProcessors());

void Obj::addLine(const string& line)
{
	try
	{
		vector<string> strs;
		boost::split(strs, line, boost::is_any_of("\t "));

		if (strs.empty() || line.empty() || line[0] == '#') return;

		ProcessorsMap::const_iterator it = Processors.find(strs[0]);
		if (it == Processors.end())
			LOG_DEBUG(std::string("unknown line prefix: " + strs[0]));
		else
			CALL_MEMBER_FN(*this, it->second)(strs);
	}
	catch(InvalidObjFormat& iof)
	{
		iof.setLine(line);
		throw iof;
	}
}


void Obj::clear()
{
	vertexes_.clear();
	textures_.clear();
	normals_.clear();
	vertexesIndices_.clear();
	texturesIndices_.clear();
	normalsIndices_.clear();
}

void Obj::processVertex(const std::vector<std::string>& data)
{
	const std::string name = "vertex (v)";

	if (data.size() != 4 && data.size() < 5)
		throw InvalidObjFormat((boost::format("incorrect arguments count in %s") % name).str());

	try
	{
		GLfloat x = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat y = boost::lexical_cast<GLfloat>(data[2]);
		GLfloat z = boost::lexical_cast<GLfloat>(data[3]);
		GLfloat w = data.size() == 5 ? boost::lexical_cast<GLfloat>(data[4]) : 1.0f;

		vertexes_.push_back(vec4(x, y, z, w));
	}
	catch (boost::bad_lexical_cast const&)
	{
		throw InvalidObjFormat((boost::format("bad argument value in %s") % name).str());
	}
}

void Obj::processVertexTexture(const std::vector<std::string>& data)
{
	const std::string name = "vertex texture (vt)";

	if (data.size() < 2 && data.size() > 4)
		throw InvalidObjFormat((boost::format("incorrect arguments count in %s") % name).str());

	try
	{
		GLfloat u = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat v = data.size() > 2 ? boost::lexical_cast<GLfloat>(data[2]) : 0.0f;
		GLfloat w = data.size() > 3 ? boost::lexical_cast<GLfloat>(data[3]) : 0.0f;

		textures_.push_back(vec3(u, v, w));
	}
	catch (boost::bad_lexical_cast const&)
	{
		throw InvalidObjFormat((boost::format("bad argument value in %s") % name).str());
	}
}

void Obj::processVertexNormal(const std::vector<std::string>& data)
{
	const std::string name = "vertex normal (vn)";

	if (data.size() != 4)
		throw InvalidObjFormat((boost::format("incorrect arguments count in %s") % name).str());

	try
	{
		GLfloat x = boost::lexical_cast<GLfloat>(data[1]);
		GLfloat y = boost::lexical_cast<GLfloat>(data[2]);
		GLfloat z = boost::lexical_cast<GLfloat>(data[3]);

		normals_.push_back(glm::normalize(vec3(x, y, z)));
	}
	catch (boost::bad_lexical_cast const&)
	{
		throw InvalidObjFormat((boost::format("bad argument value in %s") % name).str());
	}
}

void Obj::processFace(const std::vector<std::string>& data)
{
	const std::string name = "face (f)";

	// only triangles
	if (data.size() != 4)
		throw InvalidObjFormat((boost::format("incorrect arguments count in %s") % name).str());

	for (std::size_t i = 1; i < data.size(); ++i)
	{
		std::vector<std::string> strs;
		boost::split(strs, data[i], boost::is_any_of("/"));

		try
		{
			GLuint vertexIdx  = boost::lexical_cast<GLuint>(strs[0]);
			GLuint textureIdx = (strs.size() > 1 && !strs[1].empty()) ? boost::lexical_cast<GLuint>(strs[1]) : 0;
			GLuint normalIdx  = strs.size() > 2 ? boost::lexical_cast<GLuint>(strs[2]) : 0;

			vertexesIndices_.push_back(vertexIdx  - 1);
			if (textureIdx > 0)
				texturesIndices_.push_back(textureIdx - 1);
			if (normalIdx > 0)
				normalsIndices_.push_back( normalIdx  - 1);
		}
		catch (boost::bad_lexical_cast const&)
		{
			throw InvalidObjFormat((boost::format("bad argument value in %s") % name).str());
		}		
	}
}


Obj::ProcessorsMap Obj::InitProcessors()
{
	ProcessorsMap map;
	map["v"]  = &Obj::processVertex;
	map["vt"] = &Obj::processVertexTexture;
	map["vn"] = &Obj::processVertexNormal;
	map["f"]  = &Obj::processFace;
	return map;
}


istream& operator>>(istream& is, Obj& obj)
{
	while (!is.eof() && !is.bad() && !is.fail())
	{
		string line;
		std::getline(is, line);
		if (!line.empty())
			obj.addLine(line);
	}
	obj.postProcessing();
	return is;
}
