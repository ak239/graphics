#version 330 core

in vec3 UVW;
out vec4 color;

uniform samplerCube textureSampler;

void main(){
//	color = vec4(vec3(UV.y), 1.0f);
  color = texture( textureSampler, UVW);
	color.w = 1.0f;
}