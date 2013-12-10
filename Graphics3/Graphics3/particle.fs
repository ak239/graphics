#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D textureSampler;

void main(){
	color = texture2D(textureSampler, texCoord);
	//if (color.w < 0.5) color.w = 0.0;
	color.w = (0.2126*color.x) + (0.7152*color.y) + (0.0722*color.z);
}