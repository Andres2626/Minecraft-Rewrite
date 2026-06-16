#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aUV;

uniform mat4 s_VP;
uniform mat4 s_M;

out vec2 uv; 

void main() 
{
    uv = aUV;
	gl_Position = s_VP * s_M * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D t1;

void main() 
{
	FragColor = texture(t1, uv);
}