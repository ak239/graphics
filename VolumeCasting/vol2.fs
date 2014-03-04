#version 330

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

const float INV_289 = 0.00346020761;

vec3 mod289(vec3 x) 
{
  return x - floor(x * INV_289) * 289.0;
}

vec2 mod289(vec2 x)
{
  return x - floor(x * INV_289) * 289.0;
}

vec3 permute(vec3 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

uniform int turbulenceCount;

uniform float initFreq;

float turbulence(in vec2 v) {
  vec2 freq = vec2(initFreq);
  float sum = 0.0;
  float pow2 = 0.5;
  for (int i = 0; i < turbulenceCount; i++) {
    sum += snoise(freq*v) * pow2;
	pow2 *= 0.5;
    freq *= vec2(2.0);
  }
  
  return sum;
}

out vec4 color;
in vec3 vPos;
uniform vec3 camPos;
uniform float stepSize;

const float Z_WIDTH = 1.0;
const float XY_WIDTH = 2.0;

void main(){
	vec3 dir   = normalize(vPos - camPos);
	vec3 start = vPos;
		
	vec3 step = dir;
	float k = stepSize / length(dir);
	step = step * k;
	
	vec3 curPos = vPos;
		
	vec4 col = vec4(0.0f);
	
	float alpha_acc = 0.0f;
	float fstY = -10.0;
	float col_val = 0.0f;
	float delta = length(step);
	
	while (true)
	{
		float top = snoise(curPos.xz * 0.5) * 0.25; //[0 .. 0.5]
		if (top > 0.0)
		{
			float bot = -top * 0.25; //[-0.25 .. 0]
			float y = curPos.y;
			if (top > bot && y < top && y > bot && top - bot > 0.02)
			{
				float color_sample = 1.0;
				float alpha_sample = top  * 10.0 * delta;
				col_val += (1.0 - alpha_acc) * color_sample * alpha_sample * 3.0;
				alpha_acc += alpha_sample;
				//col_val = 1.0;
				//alpha_acc = 1.0;
			}
		}
		curPos += step;
		if (alpha_acc > 0.9){ col.a = 1.0; break; }
		if (abs(curPos.x) > XY_WIDTH || abs(curPos.y) > Z_WIDTH || abs(curPos.z) > XY_WIDTH) break;
	}
	color = vec4(vec3(col_val), alpha_acc);
}