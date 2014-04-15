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

vec2 mod289(vec2 x) { return x - floor(x * INV_289) * 289.0; }
vec3 mod289(vec3 x) { return x - floor(x * INV_289) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * INV_289) * 289.0; }

vec3 permute(vec3 x) { return mod289(((x * 34.0) + 1.0) * x); }
vec4 permute(vec4 x) { return mod289(((x * 34.0) + 1.0) * x); }

vec3 taylorInvSqrt(vec3 r) { return 1.79284291400159 - 0.85373472095314 * r; }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

vec2 fade(vec2 t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }
vec3 fade(vec3 t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }

float snoise(vec2 v) 
{
  const vec4 C = vec4(0.211324865405187, 0.366025403784439, 
                     -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

  i = mod289(i);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

float snoise(vec3 v)
{ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

float cnoise(vec2 P)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod289(Pi); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  g00 *= norm.x;  
  g01 *= norm.y;  
  g10 *= norm.z;  
  g11 *= norm.w;  

  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

float cnoise(vec3 P)
{
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}

uniform vec3 cameraPosition;
uniform float stepSize;

const int perlinNoise2d  = 0;
const int simplexNoise2d = 1;
uniform int noise2dType;

uniform vec2 noise2dPhase;
uniform vec2 noise2dFreq;
uniform int noise2dTurbulence;

uniform float T;
uniform float Ptop;
uniform float Pbot;
uniform float ktop;
uniform float kbot;

const int calcHconst   = 0;
const int calcHtwovals = 1;
uniform int calcHtype;

uniform float H;

const int perlinNoise3d = 0;
const int simplexNoise3d = 1;
uniform int noise3dType;

uniform vec3 noise3dFreq;
uniform int noise3dTurbulence;

const int borderCalcSum = 0;
const int borderCalcMul = 0;
uniform int borderCalcType;

const int sigmoidLinear = 0;
const int sigmoid3xx = 1;
uniform int sigmoidType;

uniform float xWidthDiv2;
uniform float zWidthDiv2;

uniform float rho0;

uniform float fogHighDist;
uniform float fogLowDist;

bool IsInsideVolume(in vec3 v) {
  float yWidth = max(kbot, ktop);
  return abs(v.x) < xWidthDiv2 && (v.y > -yWidth && v.y < yWidth) && abs(v.z) < zWidthDiv2;
}

float Noise(in vec2 p) {
  if (noise2dType == simplexNoise2d) {
    return snoise(p);
  }
  else {
    return cnoise(p);
  }
}

float TurbulenceNoise(in vec2 v) {
  vec2 freq = vec2(noise2dFreq);
  float sum = 0.0;
  float pow2 = 0.5;
  for (int i = 0; i < noise2dTurbulence; i++) {
    sum += Noise(freq * v) * pow2;
    pow2 *= 0.5;
    freq *= vec2(2.0);
  }
  return sum;
}

float Noise(in vec3 p) {
  if (noise3dType == simplexNoise3d)
    return snoise(p);
  else
    return cnoise(p);
}

float TurbulenceNoise(in vec3 v) {
  vec3 freq = vec3(noise3dFreq);
  float sum = 0.0;
  float pow2 = 0.5;
  for (int i = 0; i < noise3dTurbulence; i++) {
    sum += Noise(freq * v) * pow2;
    pow2 *= 0.5;
    freq *= vec3(2.0);
  }
  return sum;
}

float CommonPart(in vec2 p) {
  if (noise2dTurbulence > 1)
    return (TurbulenceNoise(p + noise2dPhase) - T)/(1.0 - T);
  return (Noise(p * noise2dFreq + noise2dPhase) - T)/(1.0 - T);
}

float Ftop(in vec2 p, in float c) {
  return pow(c, Ptop) * ktop;
}

float Fbot(in vec2 p, in float c) {
  return - pow(c, Pbot) * kbot;
}

float Hbot(in float h) {
  if (calcHtype == calcHconst)
    return h;
  else {
    return kbot * pow(h / ktop, Pbot / Ptop);
  }
}

float Sigmoid(in float x) {
  if (sigmoidType == sigmoidLinear)
    return x;
  else
    return 3 * x * x * x + 2 * x * x;
}

out vec4 color;
in vec3 entryPoint;

void main(){
  vec3 dir  = normalize(entryPoint - cameraPosition);
  vec3 step = dir * stepSize; 
  vec3 position = entryPoint;
  vec3 nextPosition = position + step;
  
  float dist = length(entryPoint - cameraPosition);
 
  float alpha_acc = 0.0;
  if (dist >  fogLowDist)
    alpha_acc = clamp((dist - fogLowDist) / (fogHighDist - fogLowDist), 0.0, 1.0);
   
  while (IsInsideVolume(nextPosition))
  {
    vec2 position2D = vec2(position.x, position.z);
    float c = CommonPart(position2D);
    if (c > 0.0)
    {
      float fTop = Ftop(position2D, c);
      float fBot = Fbot(position2D, c);
    
      if (fBot < position.y && position.y < fTop)
      {
        float topDist = fTop - position.y;
        float botDist = position.y - fBot;
              
        float d = min(topDist, botDist);
        float h = H;
        
        if (botDist < topDist && calcHtype == calcHtwovals)
          h = kbot * pow(h / ktop, Pbot / Ptop);

        float rho = rho0;
        if (d < h)
        {
          if (noise3dTurbulence > 1)
            rho = rho0 * Sigmoid(d / h) * (TurbulenceNoise(position) + 1.0) * 0.5;
          else
            rho = rho0 * Sigmoid(d / h) * (Noise(position * noise3dFreq) + 1.0) * 0.5;
        }
          
        alpha_acc += rho;
        
        if (alpha_acc >= 1.0)
          break;
      }
    }
    
    position = nextPosition;
    nextPosition += step;
  }
  
  color = vec4(1.0, 1.0, 1.0, alpha_acc);
  return;
}