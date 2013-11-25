#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec4 Position_camera;
out vec2 UV;

uniform mat4 MV;
uniform mat4 P;

void main(){
  Position_camera = MV * vec4(vertexPosition_modelspace,1);
  gl_Position     = P * Position_camera;
  UV              = vertexUV;
}