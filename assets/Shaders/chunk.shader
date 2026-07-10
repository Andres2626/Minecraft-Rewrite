#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aBrig;

out VS_OUT
{
	vec3 pos;
    vec3 color;
	vec2 uv;
	float brightness;
	vec3 worldPos;
}vs_out;

uniform mat4 s_VP;
uniform mat4 s_M;

void main() 
{
	vec4 world = s_M * vec4(aPos, 1.0f);
	vs_out.pos = aPos;
    vs_out.color = aColor;
	vs_out.uv = aUV;
	vs_out.brightness = aBrig;
	vs_out.worldPos = world.xyz;

	gl_Position = s_VP * world;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 pos;
    vec3 color;
	vec2 uv;
	float brightness;
	vec3 worldPos;
} fs_in;

uniform vec3 s_cpos;
uniform vec4 s_fcolor0;
uniform float s_fdensity0;
uniform vec4 s_fcolor1;
uniform float s_fdensity1;

uniform sampler2D s_t1;

void main() 
{
	vec4 result = texture(s_t1, fs_in.uv) * fs_in.brightness;
	float dist = length(fs_in.worldPos - s_cpos);
    
    vec3 final;
	if (fs_in.brightness < 0.6f) {
		float fogFactor = exp(-s_fdensity1 * dist);
		final = mix(s_fcolor0.rgb, result.rgb, fogFactor);
	}
    else {
        float fogFactor = exp(-s_fdensity0 * dist);
		final = mix(s_fcolor1.rgb, result.rgb, fogFactor);
    }

    result = vec4(final, result.a);
	FragColor = result;
}