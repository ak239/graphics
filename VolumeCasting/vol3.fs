#version 330

out vec4 color;
in  vec3 vPos;

uniform vec3  camPos;
uniform float stepSize;
uniform float FX0; // = 1.0
uniform float FY0; // = 1.0
uniform float C0; // = 0.3
// max(T(x,y,z)) = 1.0
uniform float k; // = 0.2
uniform float PX0;
uniform float PY0;
uniform float n; // = 4.0
// contrast of the texture pattern
uniform float T0; // = 0.4

uniform float T1;
uniform float T2;
uniform float D;
uniform float Fmax;

uniform float a1;
uniform float a2;

const float Z_WIDTH = 3.0;
const float XY_WIDTH = 10.0;

const float PI   = 3.1415926535897932384626433832795;
const float PI_2 = PI * 0.5;

float calcT(vec3 pos)
{
	float sumX = 0.0;
	float sumY = 0.0;
	
	float curC  = C0;
	float curFX = FX0;
	float curFY = FY0;
	
	float curPX = PX0;
	float curPY = PY0;
	
	for (int i = 0; i < n; i++)
	{
		curPX = curPX + PI * sin(curFX * pos.y * 0.5);
		curPY = curPY + PI * sin(curFX * pos.y * 0.5);

		sumX += curC * sin(curFX * pos.x + curPX) + T0;
		sumY += curC * sin(curFY * pos.z + curPY) + T0;
		
		curC = 0.707 * curC;
		curFX = 2.0 * curFX;
		curFY = 2.0 * curFY;
		curPX = PI_2 * sin(0.5 * curFY * pos.z);
		curPY = PI_2 * sin(0.5 * curFX * pos.x);		
	}
	
	return k * sumX * sumY;
}

float getG(float x, float z)
{
	return abs((a1 * pow(x, 2.0) + a2 * pow(z, 2.0) - 1.0)) / Fmax;
}

void main(){
	vec3 ellipsoid_c   = vec3(0.0);
	vec3 ellipsoid_abc = vec3(XY_WIDTH, Z_WIDTH, XY_WIDTH) * 0.5;
	vec3 P1 = vPos / ellipsoid_abc;
	vec3 v  = normalize(camPos - vPos);
	vec3 v1 = v / ellipsoid_abc; 
	
	float b = 2 * dot(P1, v1);
	float a = dot(v1, v1);
	float d = pow(b, 2.0) - 4.0 * (dot(P1, P1) - 1.0) * a;
	if (d >= 0)
	{
		float sqrt_d = sqrt(d);
		vec3 fst = vPos + v * (-b + sqrt_d) / 2.0 / a;
		
		float T = calcT(fst);	
		float g = getG(fst.x, fst.y);
		
		float TR = 1.0 - (T - T1 - (T2 - T1) * (1.0 - g)) / D;
		
		color = vec4(vec3(T), TR);
	}
	else
		color = vec4(vec3(0.0), 0.0);
}