#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

/* instances */
layout(location = 5) in vec3 iPos;
layout(location = 6) in uint iTex;
layout(location = 7) in vec2 iTexOffset;

uniform mat4 s_VP;
uniform vec3 s_camRight;
uniform vec3 s_camUp;

out vec2 uv; 

void main()
{
    float size = 0.1f;
    
    vec3 worldPos = iPos + aPos.x * s_camRight * size + aPos.y * s_camUp * size;
    gl_Position = s_VP * vec4(worldPos, 1.0);
    
    /* calculate texture coords */
    float minU = (float(iTex % 16u) + iTexOffset.x / 4.0) / 16.0;
    float minV = (float(iTex / 16u) + iTexOffset.y / 4.0) / 16.0;
    float du = 999.0 / 64000.0;
    float dv = 999.0 / 64000.0;

    vec2 quadUV = aPos.xy + vec2(0.5);

    uv = vec2(minU + quadUV.x * du, minV + quadUV.y * dv);
}

#shader fragment
#version 330 core

in vec2 uv;

uniform sampler2D s_t1;
uniform vec3 s_color;
out vec4 FragColor;

void main()
{
    vec4 tex = texture(s_t1, uv);
    if (tex.a < 0.1)
        discard;
    
    FragColor = tex * vec4(s_color, 1.0);
}