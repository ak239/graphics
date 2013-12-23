#pragma once

#include "RenderObject.h"
#include "ArrayStaticBuffer.h"
#include "UniformWrapper.h"

class Emitter;
class Shader;
class Texture;
class Texture1D;

class Particles : public RenderObject
{
public:
	Particles(GLContext context);
	virtual ~Particles(void);

	void setShader(Shader* _shader);
	void setData(Emitter* emitter);
	void setTexture(Texture* _texture);
	void setColorTexture(Texture1D* _texture);

private:
	virtual void renderImpl();

	ArrayStaticBuffer<vec3>    particles_;
	ArrayStaticBuffer<vec3>    velocities_;
	ArrayStaticBuffer<GLfloat> startTimes_;
	ArrayStaticBuffer<vec3>    colors_;

	Shader*  shader_;
	Texture* texture_;
	Texture1D* color_;
	UniformWrapper<mat4> M_;
	UniformWrapper<mat4> V_;
	UniformWrapper<mat4> P_;
};

