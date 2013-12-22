#pragma once

#include <vector>
using std::vector;

class Emitter
{
public:
	Emitter(const vec3& basePoint, const vec3& baseVelocity, size_t count,
		GLfloat oneCycleTime, GLfloat baseRadius, GLfloat scatterOfVelocity,
		const vec3& fromColor = vec3(), const vec3& toColor = vec3());

	const vector<vec3>&    particles()  const{ return particles_;  }
	const vector<vec3>&    velocities() const{ return velocities_; }
	const vector<GLfloat>& startTimes() const{ return startTimes_; }
	const vector<vec3>&    colors()     const{ return color_; }
private:
	vector<vec3>    particles_;
	vector<vec3>    velocities_;
	vector<GLfloat> startTimes_;
	vector<vec3>    color_;
};

