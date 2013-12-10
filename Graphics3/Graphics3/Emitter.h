#pragma once

#include <vector>
using std::vector;

class Emitter
{
public:
	Emitter(const vec3& basePoint, const vec3& baseVelocity, size_t count,
		GLfloat oneCycleTime, GLfloat baseRadius, GLfloat scatterOfVelocity);

	const vector<vec3>&    particles()  const{ return particles_;  }
	const vector<vec3>&    velocities() const{ return velocities_; }
	const vector<GLfloat>& startTimes() const{ return startTimes_; }
private:
	vector<vec3>    particles_;
	vector<vec3>    velocities_;
	vector<GLfloat> startTimes_;
};

