#shader vertex
#version 300 es

precision highp float;
precision highp int;
precision highp sampler2D;

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aBrig;
layout (location = 4) in float aTid;

uniform mat4 s_VP;
uniform mat4 s_M;

out vec2 uv; 
out float brig; 
out vec3 color; 
out float tid;

void main() 
{
    uv = aUV;
    brig = aBrig;
    color = aColor;
    tid = aTid;
	gl_Position = s_VP * s_M * vec4(aPos, 1.0f);
}

#shader fragment
#version 300 es

precision highp float;
precision highp int;
precision highp sampler2D;

in vec2 uv;
in float brig;
in vec3 color;
in float tid;
out vec4 FragColor;

uniform sampler2D s_Textures[16];

vec4 SampleTexture(int index, vec2 uv)
{
    switch (index)
    {
        case 0: return texture(s_Textures[0], uv);
        case 1: return texture(s_Textures[1], uv);
        case 2: return texture(s_Textures[2], uv);
        case 3: return texture(s_Textures[3], uv);
        case 4: return texture(s_Textures[4], uv);
        case 5: return texture(s_Textures[5], uv);
        case 6: return texture(s_Textures[6], uv);
        case 7: return texture(s_Textures[7], uv);
        case 8: return texture(s_Textures[8], uv);
        case 9: return texture(s_Textures[9], uv);
        case 10: return texture(s_Textures[10], uv);
        case 11: return texture(s_Textures[11], uv);
        case 12: return texture(s_Textures[12], uv);
        case 13: return texture(s_Textures[13], uv);
        case 14: return texture(s_Textures[14], uv);
        case 15: return texture(s_Textures[15], uv);

        default: return vec4(1.0);
    }
}

void main()
{
    vec4 result = vec4(1.0);

    if (tid > 0.0)
        result *= SampleTexture(int(tid - 1.0), uv);

    result *= brig;

    FragColor = vec4(color, 1.0) * result;
}