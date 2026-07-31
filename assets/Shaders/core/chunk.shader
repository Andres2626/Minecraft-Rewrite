#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aBrig;

/* instances (for bushes at the moment)*/
layout(location = 5) in vec3 iPos;
layout(location = 6) in float iBrig;

out vec3 pos;
out vec3 color;
out vec2 uv;
out float brightness;
out vec3 worldPos;

uniform bool s_RenderBush;
uniform mat4 s_VP;
uniform mat4 s_M;

void main() 
{
    vec3 pos = aPos;
    if (s_RenderBush) {
        pos += iPos;
        brightness = iBrig;
    }
    else
        brightness = aBrig;
        
    vec4 world = s_M * vec4(pos, 1.0f);
	pos = aPos;
    color = aColor;
	uv = aUV;
	worldPos = world.xyz;
	gl_Position = s_VP * world;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 pos;
in vec3 color;
in vec2 uv;
in float brightness;
in vec3 worldPos;

uniform vec3 s_cpos;
uniform vec4 s_fcolor0;
uniform float s_fdensity0;
uniform vec4 s_fcolor1;
uniform float s_fdensity1;

uniform sampler2D s_t1;

void main() 
{
	vec4 tex = texture(s_t1, uv);
    if (tex.a < 0.1)
        discard;
    
    vec4 result = tex * brightness;
	float dist = length(worldPos - s_cpos);
    
    vec3 final;
	if (brightness < 0.6) {
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