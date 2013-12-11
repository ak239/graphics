#include "stdafx.h"
#include "UniformWrapper.h"

#include "Shader.h"

void useShader(Shader* shader)
{
	shader->use();
}
