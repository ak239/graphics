#version 330 core

in vec3 normal;
in vec3 lightColor;

varying float d;
varying vec3 l;
varying vec3 v;
varying vec3 n;
varying vec3 h;

out vec4 color;

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
uniform float ambientPower;

uniform vec3  diffuseColor;
uniform float diffusePower;

uniform vec3  specularColor;
uniform float specularPower;

void main(){
	if (fillType != ftPerFragment)
		color = vec4(lightColor, 1.0f);
	else
	{
		if (modelType == mtPhong)
		{
			vec3 r = reflect(-v, n);
			vec3 ambient  = ambientColor;
			vec3 diffuse  = diffuseColor * max(dot(n, l),0.0);
			vec3 specular = specularColor * pow(max(dot(l, r), 0.0), specularPower);
			
			color = vec4(ambient + (diffuse + specular) / d / d, 1.0f);			
		}
		else
		{
			vec3 ambient  = ambientColor;
			vec3 diffuse  = diffuseColor * max(dot(n, l),0.0);
			vec3 specular = specularColor * pow(max(dot(n, h), 0.0), specularPower);
			
			color = vec4(ambient + (diffuse + specular) / d / d, 1.0f);		
		}
	}
}