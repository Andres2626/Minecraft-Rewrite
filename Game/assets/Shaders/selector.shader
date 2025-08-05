#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 s_VP;
uniform mat4 s_M;

void main() 
{
	gl_Position = s_VP * s_M * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform float s_Timer;

void main() {
	float a = sin(s_Timer / 100.0f) * 0.2f + 0.4f;
	FragColor = vec4(1.0f, 1.0f, 1.0f, a);
}