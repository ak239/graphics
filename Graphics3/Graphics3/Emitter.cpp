#include "GL.h"
#include "Emitter.h"


Emitter::Emitter(const vec3& basePoint, const vec3& baseVelocity, size_t count,
		GLfloat oneCycleTime, GLfloat baseRadius, GLfloat scatterOfVelocity)
{
	for (size_t i = 0; i < count; ++i)
	{
		vec3 point = glm::ballRand(baseRadius);
		particles_.push_back(vec3(basePoint.x + point.x, basePoint.y, basePoint.z + point.z));
		velocities_.push_back(baseVelocity + glm::ballRand(scatterOfVelocity));
		startTimes_.push_back(glm::linearRand(0.0f, oneCycleTime));
	}
}
