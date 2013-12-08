#pragma once

#include <string>
#include <vector>
#include <istream>
#include <map>
#include <stdexcept>

using std::string;
using std::istream;
using std::vector;
using std::map;
using std::runtime_error;

using glm::vec4;
using glm::vec3;

struct InvalidObjFormat : public std::runtime_error
{
public:
	const string& line()   const{ return line_; }
	const size_t  number() const{ return number_; }
	const string& msg()    const{ return msg_; }

	void setLine(const string& _line){ line_ = _line; }
	void setNumber(size_t _number){ number_ = _number; }
	void setMsg(const string& _msg){ msg_ = _msg; }

	InvalidObjFormat(const string& _msg, const string& _line = string(), size_t _number = -1):
		runtime_error(_msg), line_(_line), number_(_number), msg_(_msg){}
private:
	string line_;
	size_t number_;
	string msg_;
};

class Obj
{
public:
	virtual ~Obj(){}

	void clear();

	const vector<vec4>& vertexes()  const{ return vertexes_; }
	const vector<vec3>& textures() const{ return textures_; }
	const vector<vec3>& normals()  const{ return normals_;  }
	const vector<GLuint>& vertexesIndices() const{ return vertexesIndices_; }
	const vector<GLuint>& texturesIndices() const{ return texturesIndices_; }
	const vector<GLuint>& normalsIndices()  const{ return normalsIndices_;  }

protected:
	virtual void postProcessing(){}

private:
	void addLine(const string& line);

	void processVertex(       const vector<string>& data);
	void processVertexTexture(const vector<string>& data);
	void processVertexNormal( const vector<string>& data);
	void processVertexSpace(  const vector<string>& data);
	void processFace(         const vector<string>& data);

	vector<vec4> vertexes_;
	vector<vec3> textures_;
	vector<vec3> normals_;
	vector<GLuint> vertexesIndices_;
	vector<GLuint> texturesIndices_;
	vector<GLuint> normalsIndices_;

public:
	typedef void (Obj::*Process)(const vector<string>& data);
	typedef map<string, Obj::Process> ProcessorsMap;
	static const ProcessorsMap Processors;
	static ProcessorsMap InitProcessors();

	friend istream& operator>>(istream& is, Obj& obj);
};

istream& operator>>(istream& is, Obj& obj); 
