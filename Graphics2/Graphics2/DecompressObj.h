#pragma once

#include "Obj.h"

class DecompressObj : public Obj
{
public:
	const vector<vec4>& decompressVertexes() const{ return vertexes_; }
	const vector<vec3>& decompressTextures() const{ return textures_; }
	const vector<vec3>& decompressNormals()  const{ return normals_;  }

protected:
	virtual void postProcessing();
	
private:
	void decompress();

	vector<vec4> vertexes_;
	vector<vec3> textures_;
	vector<vec3> normals_;
};

