#version 330 core

layout(location = 0) in vec3 Position;

out vec3 UVW;

uniform mat4 uWVP;

void main(){
  gl_Position = uWVP * vec4(Position, 1);
  UVW = Position;
}