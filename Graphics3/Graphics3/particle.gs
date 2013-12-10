#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

out vec2 texCoord;

uniform vec3  cameraPos;
uniform float size;

varying int   textureIndex[];

void main()
{
  for(int i = 0; i < gl_in.length(); i++) 
	{
		int col = textureIndex[i] / 4;
		int row = textureIndex[i] % 4;
	
		vec3 toCamera = normalize(cameraPos - gl_in[i].gl_Position.xyz);
    vec3 up       = vec3(0.0, 1.0, 0.0);
    vec3 right    = normalize(cross(toCamera, up));
				
		vec4 Pos = vec4(gl_in[i].gl_Position.xyz - (right * 0.5 * size) - up * 0.5 * size, gl_in[i].gl_Position.w);
		gl_Position = Pos;
		texCoord = vec2(row * 0.25, 1.0 - col * 0.25);
    EmitVertex();
		
		Pos.y += size;
		gl_Position = Pos;
		texCoord = vec2(row * 0.25, 1.0 - (col + 1) * 0.25);
    EmitVertex();
		
		Pos.x += size;
		Pos.y -= size;
		gl_Position = Pos;
		texCoord = vec2((row + 1) * 0.25, 1.0 - col * 0.25);
    EmitVertex();
		
		Pos.y += size;
		gl_Position = Pos;
		texCoord = vec2((row + 1) * 0.25, 1.0 - (col + 1) * 0.25);
    EmitVertex();

		EndPrimitive();
  }
}