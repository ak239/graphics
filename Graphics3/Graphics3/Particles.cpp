#include "Particles.h"

#include "Emitter.h"
#include "VertexAttribArray.h"
#include "Shader.h"
#include "Texture.h"

Particles::Particles(GLContext context) : RenderObject(context), shader_(0)
{
}


Particles::~Particles(void)
{
}


void Particles::setShader(Shader* _shader)
{
	shader_ = _shader;
	if (shader_)
	{
		shader_->use();
		M_ = shader_->getUniform<mat4>("M");
		V_ = shader_->getUniform<mat4>("V");
		P_ = shader_->getUniform<mat4>("P");
	}
}


void Particles::setData(Emitter* emitter)
{
	particles_.bufferData( emitter->particles());
	velocities_.bufferData(emitter->velocities());
	startTimes_.bufferData(emitter->startTimes());
}


void Particles::renderImpl()
{
	if (shader_)
	{
		shader_->use();

		M_.setValue(getModel());
		V_.setValue(getCameraView());
		P_.setValue(getProjection());
	}
	
	glDisable(GL_DEPTH_TEST);

	texture_->activeAndBind();

	VertexAttribArray<vec3>    vertexesAttrib(  particles_,  0);
	VertexAttribArray<vec3>    velocitiesAttrib(velocities_, 1);
	VertexAttribArray<GLfloat> startTimesAttrib(startTimes_, 2);

	glDrawArrays(GL_POINTS, 0, particles_.count());

	glEnable (GL_DEPTH_TEST);
	
	OPENGL_CHECK_FOR_ERRORS();
}

void Particles::setTexture(Texture* _texture)
{
	texture_ = _texture;
}
