#version 330 core

in vec2 UV;
in vec4 Position_camera;
out vec4 color;

uniform sampler2D textureSampler;
uniform vec2 farNear;
uniform int  zCalcType;
uniform vec2 windowSize;
uniform mat4 invP;
uniform int  polygonMode;
uniform float uvScale;

vec3 CalcPositionFromFragCoord()
{
  vec4 ndcPos;
  ndcPos.xy = ((gl_FragCoord.xy / windowSize.xy) * 2.0) - 1.0;
  ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
      (gl_DepthRange.far - gl_DepthRange.near);
  ndcPos.w = 1.0;

  vec4 clipPos = ndcPos / gl_FragCoord.w;

  return vec3(invP * clipPos);
}

float CalcDepth(float z)
{
  return (-z - farNear.y) / (farNear.x - farNear.y);
}



void main(){
    //color = vec4(vec3(UV.x), 1.0f);
    color = texture2D( textureSampler, UV * uvScale );
}