#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aBrig;

uniform mat4 s_VP;
uniform mat4 s_M;

out vec2 uv; 
out float brig; 
out vec3 color; 

void main() 
{
    uv = aUV;
    brig = aBrig;
    color = aColor;
	gl_Position = s_VP * s_M * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

in vec2 uv;
in float brig;
in vec3 color;
out vec4 FragColor;

uniform int s_RenderFlags;
uniform sampler2D s_t1;

/* render flags defines */
#define SHADER_COLOR_FLAG    (1 << 1)
#define SHADER_TEXTURE_FLAG  (1 << 2)
#define SHADER_BRIG_FLAG     (1 << 3)

void main() 
{
    vec4 result = vec4(1.0f);

    if ((s_RenderFlags & SHADER_COLOR_FLAG) != 0)
        result *= vec4(color, 1.0);

    if ((s_RenderFlags & SHADER_TEXTURE_FLAG) != 0)
        result *= texture(s_t1, uv);

    if ((s_RenderFlags & SHADER_BRIG_FLAG) != 0)
        result.rgb *= brig;

    FragColor = result;
}