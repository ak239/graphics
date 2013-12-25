#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

out vec2 texCoord;
out vec2 nextTexCoord;
out vec3 particleColor;
out float fsCurrentStage;
out float fsCurrentInStage;
out float fsTextureIdx;

uniform vec3  cameraPos;

in int   textureIndex[];
in vec3  currentColor[];
in float currentStage[];
in float currentInStage[];
in float curSize[];

void main()
{
  for(int i = 0; i < gl_in.length(); i++) 
	{
		int col = textureIndex[i] / 4;
		int row = textureIndex[i] % 4;
		int colNext = (textureIndex[i] + 1) / 4;
		int rowNext = (textureIndex[i] + 1) % 4;
		if (textureIndex[i] + 1 == 16)
		{
			rowNext = 3; colNext = 3;
		}
		
		vec3 toCamera = normalize(cameraPos - gl_in[i].gl_Position.xyz);
    vec3 up       = vec3(0.0, 1.0, 0.0);
    vec3 right    = normalize(cross(toCamera, up));
				
		float size = curSize[i];
		
		vec4 Pos = vec4(gl_in[i].gl_Position.xyz - (right * 0.5 * size) - up * 0.5 * size, gl_in[i].gl_Position.w);
		gl_Position = Pos;
		texCoord = vec2(row * 0.25, 1.0 - col * 0.25);
		nextTexCoord = vec2(rowNext * 0.25, 1.0 - colNext * 0.25);
		particleColor = currentColor[i];
		fsCurrentStage = currentStage[i];
		fsCurrentInStage = currentInStage[i];
		fsTextureIdx = float(textureIndex[i]);
    EmitVertex();
		
		Pos.y += size;
		gl_Position = Pos;
		texCoord = vec2(row * 0.25, 1.0 - (col + 1) * 0.25);
		nextTexCoord = vec2(rowNext * 0.25, 1.0 - (colNext + 1) * 0.25);
		particleColor = currentColor[i];
		fsCurrentStage = currentStage[i];
		fsCurrentInStage = currentInStage[i];
		fsTextureIdx = textureIndex[i];
		fsTextureIdx = float(textureIndex[i]);
    EmitVertex();
		
		Pos.x += size;
		Pos.y -= size;
		gl_Position = Pos;
		texCoord = vec2((row + 1) * 0.25, 1.0 - col * 0.25);
		nextTexCoord = vec2((rowNext + 1) * 0.25, 1.0 - colNext * 0.25);
		particleColor = currentColor[i];
		fsCurrentStage = currentStage[i];
		fsCurrentInStage = currentInStage[i];
		fsTextureIdx = textureIndex[i];
		fsTextureIdx = float(textureIndex[i]);
    EmitVertex();
		
		Pos.y += size;
		gl_Position = Pos;
		texCoord = vec2((row + 1) * 0.25, 1.0 - (col + 1) * 0.25);
		nextTexCoord = vec2((rowNext + 1) * 0.25, 1.0 - (colNext + 1) * 0.25);
		particleColor = currentColor[i];
		fsCurrentStage = currentStage[i];
		fsCurrentInStage = currentInStage[i];
		fsTextureIdx = textureIndex[i];
		fsTextureIdx = float(textureIndex[i]);
    EmitVertex();

		EndPrimitive();
  }
}