#version 330 core

layout(location = 0) in vec3  vertexPosition_modelspace;
layout(location = 1) in vec3  velocity;
layout(location = 2) in float startTime;
layout(location = 3) in vec3  color;

uniform mat4  M;
uniform mat4  V;
uniform mat4  P;
uniform float time;
uniform float cycleTime;

varying int   textureIndex;
varying vec3  currentColor;
varying float currentStage;

void main()
{
	vec4 worldPosition = M * vec4(vertexPosition_modelspace,1);
	
	float curTime   = time;
	float deltaTime = mod(time - startTime, cycleTime) / 1000.0;
	currentStage = mod(time - startTime, cycleTime) / cycleTime;
	vec3  s = vec3(0.0);
	if (curTime > startTime)
		s = deltaTime * velocity * exp(- 0.5 * currentStage);
	
	if (curTime > startTime)
		textureIndex = int(deltaTime * 1000.0 / cycleTime * 16.0) % 16;
	else
		textureIndex = 0;
		
	currentColor = color;
  gl_Position = P * V * (worldPosition + vec4(s, 0.0));	
}