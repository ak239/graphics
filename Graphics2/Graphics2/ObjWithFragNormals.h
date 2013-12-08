#pragma once

#include "DecompressObj.h"

class ObjWithFragNormals : public DecompressObj
{
public:
	const vector<vec3> fragNormals() const{ return fragNormals_; }

protected:
	virtual void postProcessing();

private:
	void calcFragNormals();

	vector<vec3> fragNormals_;
};

