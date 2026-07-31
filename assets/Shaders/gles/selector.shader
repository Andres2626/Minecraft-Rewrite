#shader vertex
#version 300 es

precision highp float;
precision highp int;
precision highp sampler2D;

layout(location = 0) in vec3 aPos;

uniform mat4 s_VP;
uniform mat4 s_M;

void main() 
{
	gl_Position = s_VP * s_M * vec4(aPos, 1.0);
}

#shader fragment
#version 300 es

precision highp float;
precision highp int;
precision highp sampler2D;

out vec4 FragColor;

uniform float s_Timer;

void main() 
{
	float a = sin(s_Timer / 100.0) * 0.2 + 0.4;
	FragColor = vec4(1.0, 1.0, 1.0, a);
}