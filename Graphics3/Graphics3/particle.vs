#version 330 core

layout(location = 0) in vec3  vertexPosition_modelspace;
layout(location = 1) in vec3  velocity;
layout(location = 2) in float startTime;
layout(location = 3) in vec3  color;
layout(location = 4) in vec2  size;
layout(location = 5) in float A;

uniform mat4  M;
uniform mat4  V;
uniform mat4  P;
uniform float time;
uniform float cycleTime;

out int   textureIndex;
out vec3  currentColor;
out float currentStage;
out float currentInStage;
out float curSize;

void main()
{
	vec4 worldPosition = M * vec4(vertexPosition_modelspace,1);
	
	float curTime   = time;
	float deltaTime = mod(time - startTime, cycleTime) / 1000.0;
	currentStage = mod(time - startTime, cycleTime) / cycleTime;
	vec3  s = vec3(0.0);
	if (curTime > startTime)
		s = deltaTime * velocity * exp(- 0.5 * currentStage);
		
	currentInStage = mod(mod(time - startTime, cycleTime) / cycleTime, 1.0 / 16) * 16; 
	
	if (curTime > startTime)
		textureIndex = int(currentStage * 16.0) % 16;
	else
		textureIndex = 0;
		
	currentColor = color;
	float baseSize = size.x * (1 - currentStage) + size.y * currentStage;
	curSize = baseSize * pow(currentStage, A);
	
  gl_Position = P * V * (worldPosition + vec4(s, 0.0));	
}