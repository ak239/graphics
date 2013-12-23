#version 330 core

out vec4 color;

in vec2  texCoord;
in vec2  nextTexCoord;
in vec3  particleColor;
in float fsCurrentStage;

uniform sampler2D textureSampler;
uniform sampler1D colorSampler;

void main(){
	vec3 tmp1 = texture2D(textureSampler, texCoord).xyz;
	vec3 tmp2 = texture2D(textureSampler, nextTexCoord).xyz;
	if (nextTexCoord.x > 1.0f)
		tmp2 = vec3(0.0f);
	vec3 col  = texture(colorSampler, fsCurrentStage).xyz;
	color.xyz = (tmp1.xyz * fsCurrentStage + tmp2.xyz * (1 - fsCurrentStage)) * col;

	// get one coordinate to alpha and mul this to sin
	color.w = (tmp1.x * fsCurrentStage + tmp2.x * (1 - fsCurrentStage)) * sin(fsCurrentStage * 3.14159265);
	
	//color = vec4(vec3(col.x), 1);
}