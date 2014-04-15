#version 330 core

in vec3 UVW;
out vec4 color;

uniform samplerCube uSkyboxTexture;

void main(){
  color = texture( uSkyboxTexture, normalize(UVW));
  color.w = 1.0f;
}