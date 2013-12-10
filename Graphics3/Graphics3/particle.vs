#version 330 core

layout(location = 0) in vec3  vertexPosition_modelspace;
layout(location = 1) in vec3  velocity;
layout(location = 2) in float startTime;

uniform mat4  M;
uniform mat4  V;
uniform mat4  P;
uniform float time;
uniform float cycleTime;

varying int   textureIndex;

void main()
{
	vec4 worldPosition = M * vec4(vertexPosition_modelspace,1);
	
	float curTime   = time;
	float deltaTime = mod(time - startTime, cycleTime) / 1000.0;
	vec3  s = vec3(0.0);
	if (curTime > startTime)
		s = deltaTime * velocity;
	
	if (curTime > startTime)
		textureIndex = int(deltaTime * 1000.0 / cycleTime * 16.0) % 16;
	else
		textureIndex = 0;
		
  gl_Position = P * V * (worldPosition + vec4(s, 0.0));	
}