#shader vertex
#version 330 core

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
#version 330 core

in vec2 uv;
in float brig;
in vec3 color;
in float tid;
out vec4 FragColor;

uniform sampler2D s_Textures[32];

void main()
{
    vec4 result = vec4(1.0);

    if (tid > 0.0)
        result *= texture(s_Textures[int(tid - 1.0)], uv);

    result *= brig;

    FragColor = vec4(color, 1.0) * result;
}