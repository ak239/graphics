#pragma once

#include <vector>
using std::vector;

class Emitter
{
public:
	Emitter(const vec3& basePoint, const vec3& baseVelocity, size_t count,
		GLfloat oneCycleTime, GLfloat baseRadius, GLfloat scatterOfVelocity,
		const vec3& fromColor = vec3(), const vec3& toColor = vec3(),
		const vec2& sizesRange = vec2(0.25, 0.25), GLfloat A = 1.0f);

	const vector<vec3>&    particles()  const{ return particles_;  }
	const vector<vec3>&    velocities() const{ return velocities_; }
	const vector<GLfloat>& startTimes() const{ return startTimes_; }
	const vector<vec3>&    colors()     const{ return color_; }

	const vector<vec2>&    sizes() const{ return sizes_; }
	const vector<GLfloat>& A()     const{ return A_; }

private:
	vector<vec3>    particles_;
	vector<vec3>    velocities_;
	vector<GLfloat> startTimes_;
	vector<vec3>    color_;

	vector<vec2>    sizes_;
	vector<GLfloat> A_;
};

