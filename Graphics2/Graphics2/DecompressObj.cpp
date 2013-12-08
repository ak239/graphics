#include "stdafx.h"
#include "DecompressObj.h"

void DecompressObj::postProcessing()
{
	Obj::postProcessing();
	decompress();
}

void DecompressObj::decompress()
{
	const vector<vec4>& allVertexes = vertexes();
	const vector<vec3>& allTextures = textures();
	const vector<vec3>& allNormals  = normals();

	for (size_t i = 0; i < vertexesIndices().size(); ++i)
		vertexes_.push_back(allVertexes[vertexesIndices()[i]]);
	for (size_t i = 0; i < texturesIndices().size(); ++i)
	{
		std::cout << texturesIndices()[i] << std::endl;
		textures_.push_back(allTextures[texturesIndices()[i]]);
	}
	for (size_t i = 0; i < normalsIndices().size(); ++i)
		normals_.push_back(allNormals[normalsIndices()[i]]);
}
