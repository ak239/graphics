#version 330

layout(location = 0) in vec3 pos;

uniform mat4 uWVP;
uniform mat4 scaleMatrix;
out vec3 entryPoint;

void main()
{
	gl_Position = uWVP * vec4(pos, 1);
	entryPoint = (vec4(pos, 1) * scaleMatrix).xyz;
}
