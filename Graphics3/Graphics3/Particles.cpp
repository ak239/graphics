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
	colors_.bufferData(emitter->colors());
	sizes_.bufferData(emitter->sizes());
	A_.bufferData(emitter->A());
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
	color_->active(GL_TEXTURE1);
	color_->bind(GL_TEXTURE1);

	VertexAttribArray<vec3>    vertexesAttrib(  particles_,  0);
	VertexAttribArray<vec3>    velocitiesAttrib(velocities_, 1);
	VertexAttribArray<GLfloat> startTimesAttrib(startTimes_, 2);
	VertexAttribArray<vec3>    colorsAttrib(    colors_,     3);
	VertexAttribArray<vec2>    sizesAttrib(     sizes_,      4);
	VertexAttribArray<GLfloat> aAttrib(A_, 5);

	glDrawArrays(GL_POINTS, 0, particles_.count());

	glEnable (GL_DEPTH_TEST);
	
	OPENGL_CHECK_FOR_ERRORS();
}

void Particles::setTexture(Texture* _texture)
{
	texture_ = _texture;
}

void Particles::setColorTexture(Texture1D* _texture)
{
	color_ = _texture;
}
