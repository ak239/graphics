#include "stdafx.h"
#include "ObjWithFragNormals.h"

void ObjWithFragNormals::postProcessing()
{
	DecompressObj::postProcessing();
	calcFragNormals();
}

void ObjWithFragNormals::calcFragNormals()
{
	using namespace glm;
	const vector<vec4>& allVertexes = decompressVertexes();
	for (size_t i = 0; i < allVertexes.size(); i += 3)
	{
		vec4 v1 = (allVertexes[i + 1] - allVertexes[i]);
		vec4 v2 = (allVertexes[i + 2] - allVertexes[i]);
		vec3 normal = normalize(cross(vec3(v1.x, v1.y, v1.z), 
			vec3(v2.x, v2.y, v2.z)));

		for (size_t j = 0; j < 3; ++j)
			fragNormals_.push_back(normal);
	}
}
