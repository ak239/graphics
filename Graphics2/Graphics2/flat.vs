#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexFragmentNormal;

out vec3 normal;
out vec3 lightColor;
out vec3 fromLight;

varying float d;
varying vec3 l;
varying vec3 v;
varying vec3 n;
varying vec3 h;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform int fillType;

const int ftFlat        = 1;
const int ftPerVerrtex  = 2;
const int ftPerFragment = 3;
	
uniform int modelType;

const int mtPhong = 1;
const int mtBlinnPhong = 2;	

uniform vec3  pointColor;
uniform vec3  pointPosition;
uniform float pointAttenuation;

uniform vec3  ambientColor;
uniform vec3  diffuseColor;

uniform vec3  specularColor;
uniform float specularPower;

uniform vec3 cameraPos;
uniform samplerCube gCubemapTexture;

void main()
{
	if (fillType == ftFlat)
		normal = vertexFragmentNormal;  
  else
		normal = vertexNormal;

	vec3 p = (M * vec4(vertexPosition_modelspace,1)).xyz;
	d = length(pointPosition - p) * pointAttenuation;
	l = normalize(pointPosition - p);
	v = normalize(cameraPos - p);
	// mat3(MV) - i don't have different scale on different axis
	//n = normalize(mat3(V * M) * normal);
	n = normalize(normal);
	h = (l + v) / length(l + v);
	
	vec3 cubeColor = texture(gCubemapTexture, normalize(-l)).xyz;
	if (fillType != ftPerFragment)
	{
		if (modelType == mtPhong)
		{
			vec3 r = reflect(v, n);
			vec3 ambient  = ambientColor;
			vec3 diffuse  = cubeColor * max(dot(n, l),0.0);
			vec3 specular = clamp(cubeColor * pow(max(dot(l, r), 0.0), specularPower), 0.0, 1.0);
			lightColor = ambient + (diffuse + specular) / d / d;
		}
		else
		{
			vec3 ambient  = ambientColor;
			vec3 diffuse  = cubeColor * max(dot(n, l),0.0);
			vec3 specular = clamp(cubeColor * pow(max(dot(n, h), 0.0), specularPower), 0.0, 1.0);
			
			//lightColor = vec3(n);
			lightColor = ambient + (diffuse + specular) / d / d;
		}
	}
  gl_Position = P * V * M * vec4(vertexPosition_modelspace,1);
}