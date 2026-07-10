#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aUV;

uniform mat4 s_VP;
uniform mat4 s_M;

out vec2 uv; 
out vec3 worldPos;

void main() 
{
    vec4 world = s_M * vec4(aPos, 1.0f);
    worldPos = world.xyz;
    uv = aUV;
	gl_Position = s_VP * world;
}

#shader fragment
#version 330 core

in vec2 uv;
in vec3 worldPos;
out vec4 FragColor;

uniform vec3 s_cpos;
uniform vec4 s_fcolor0;
uniform float s_fdensity0;
uniform vec4 s_fcolor1;
uniform float s_fdensity1;

uniform sampler2D s_t1;
uniform float s_brightness;

void main() 
{
    vec4 result = texture(s_t1, uv) * s_brightness;
	float dist = length(worldPos - s_cpos);
    
    vec3 final;
    if (s_brightness < 0.6f) {
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