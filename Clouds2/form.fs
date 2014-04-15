#version 330

const float INV_289 = 0.00346020761;

vec3 mod289(vec3 x) 
{
  return x - floor(x * INV_289) * 289.0;
}

vec2 mod289(vec2 x)
{
  return x - floor(x * INV_289) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * INV_289) * 289.0;
}

vec3 permute(vec3 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec4 permute(vec4 x) {
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
  vec4 x12 = x0.xyxy + C.xxzz
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

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
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

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

uniform float initFreq;

const int turbulenceCount = 2;

float turbulence(in vec2 v) {
  vec2 freq = vec2(initFreq);
  float sum = 0.0;
  float pow2 = 0.5;
  for (int i = 0; i < turbulenceCount; i++) {
    sum += snoise(freq*v) * pow2;
    pow2 *= 0.5;
    freq *= vec2(2.0);
  }
  
  return sum * 0.5 + 0.5;
}

const int cloudTurbulenceCount = 2;
uniform float initFreqCloud;

float turbulence(in vec3 v) {
  vec3 freq = vec3(initFreqCloud);
  float sum = 0.0;
  float pow2 = 0.5;
  for (int i = 0; i < cloudTurbulenceCount; i++) {
    sum += snoise(freq * v) * pow2;
    pow2 *= 0.5;
    freq *= vec3(2.0);
  }
  
  return sum * 0.5 + 0.5;
}

out vec4 color;
in vec3 vPos;
uniform vec3 camPos;
uniform float stepSize;
uniform float kTop;
uniform float kBot;
uniform float freq;
uniform float colKt;

uniform float deltaX;
uniform float deltaY;
uniform float freq2;

uniform float T;
uniform float P1;
uniform float P2;

uniform float alpha;
uniform float minNoise;
uniform float maxNoise;

uniform float h;
uniform float rho0;

uniform float alpha2;

const float Y_WIDTH = 3.0;
const float XZ_WIDTH = 10.0;

bool IsInsideVolume(in vec3 v)
{
  return abs(v.x) < XZ_WIDTH && abs(v.y) < Y_WIDTH && abs(v.z) < XZ_WIDTH;
}

bool IsInsideCloud(in vec3 v)
{
  float noiseValue = turbulence(v.xz);
  
  float afterTsmall = abs((noiseValue - T) / (1 - T));
  float topSmall =  pow(afterTsmall, P1) * kTop;
  float botSmall = -pow(afterTsmall, P2) * kBot;
  
  return v.y < topSmall && v.y > botSmall && noiseValue < T;
}

float signoid(in float x)
{
  return 2.0 / (1.0 + exp(-x)) - 1.0;
  //return x / (1 + abs(x));
}

void main(){
  vec3 dir   = normalize(vPos - camPos);
   
  vec3 step = dir;
  float k = stepSize / length(dir);
  step = step * k;
  
  vec3 curPos = vPos;
      
  float alpha_acc = 0.0f;
  
  while (IsInsideVolume(curPos + step))
  {
    if (abs(curPos.y - snoise(curPos.xy)) < 0.05)
      alpha_acc = 1.0;
    if (alpha_acc > 0.9){ col_val = 1.0; col.a = 0.0; break; }
  }
  color = vec4(vec3((curPos.y + 1.0) / 2.0), alpha_acc);
}