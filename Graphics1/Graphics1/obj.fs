#version 330 core

in vec4 Position_camera;
out vec4 color;

uniform vec2 farNear;
uniform int  zCalcType;
uniform vec2 windowSize;
uniform mat4 invP;
uniform int  polygonMode;

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
    if (polygonMode == 0)
    {
        color = vec4(1.0f);
        return;
    }

    if (zCalcType == 1)
        color = vec4(0.0f, 0.0f, CalcDepth(Position_camera.z), 1.0f);
    else if (zCalcType == 2)
        color = vec4(0.0f, CalcDepth(CalcPositionFromFragCoord().z), 0.0f, 1.0f);
    else
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}