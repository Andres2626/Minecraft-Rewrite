#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aBrig;

out VS_OUT{
	vec3 pos;
	vec2 uv;
	float brightness;
	vec3 worldPos;
}vs_out;

uniform mat4 s_VP;
uniform mat4 s_M;

void main() {
	vec4 world = s_M * vec4(aPos, 1.0f);
	vs_out.pos = aPos;
	vs_out.uv = aUV;
	vs_out.brightness = aBrig;
	vs_out.worldPos = world.xyz;

	gl_Position = s_VP * world;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT{
	vec3 pos;
	vec2 uv;
	float brightness;
	vec3 worldPos;
} fs_in;

uniform vec3 s_cpos;
uniform vec4 s_fcolor;
uniform float s_fstart;
uniform float s_fend;

uniform sampler2D t1;

void main() {
	vec4 result = texture(t1, fs_in.uv) * fs_in.brightness;

	/* TODO: Improve this! (if cycle) */ 
	if (fs_in.brightness < 0.6f) {
		float dist = length(fs_in.worldPos - s_cpos);

		float fogFactor = clamp((s_fend - dist) / (s_fend - s_fstart), 0.0, 1.0);
		vec3 final = mix(s_fcolor.rgb, result.rgb, fogFactor);
		result = vec4(final, result.a);
	}

	FragColor = result;
}
