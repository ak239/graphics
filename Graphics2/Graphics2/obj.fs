#version 330 core

in vec3 normal;
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
    vec3 L = normalize(vec3(0.0, 10.0, 0.0) - Position_camera.xyz);
    vec3 E = normalize(-L);
    vec3 R = normalize(reflect(-normalize(L), normalize(normal)));
    float spec = clamp(dot(normalize(normal), R), 0.0, 1.0); 
    spec = pow(spec, 32.0);
    color = vec4(vec3(spec), 1.0);
}