#version 330 core

out vec4 color;

in vec2  texCoord;
in vec3  particleColor;
in float fsCurrentStage;

uniform sampler2D textureSampler;

void main(){
	vec3 tmp = texture2D(textureSampler, texCoord).xyz;
	color.xyz = tmp * particleColor;
	//if (color.w < 0.5) color.w = 0.0;
	color.w = (0.2126*tmp.x) + (0.7152*tmp.y) + (0.0722*tmp.z);
	color.w = exp(-fsCurrentStage) * (1.0 - texture2D(textureSampler, texCoord).w);
	//color.w = exp(-fsCurrentStage);
	
	//color = vec4(vec3(color.w), 1.0);
}