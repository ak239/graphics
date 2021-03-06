#version 330

layout(location = 0) in vec3 pos;

uniform mat4 MVP;
out vec3 vPos;

void main()
{
	gl_Position = MVP * vec4(pos, 1);
	vPos = pos;
}
