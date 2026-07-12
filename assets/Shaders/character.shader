#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aUV;

/* instances */
layout(location = 5) in vec3 iPos;
layout(location = 6) in float iRot;
layout(location = 7) in float iTimeOff;
layout(location = 8) in float iSpeed;
layout(location = 9) in float iBrig;

uniform mat4 s_VP;
uniform float s_seconds;
uniform float s_scale;
uniform int s_partindex;
uniform vec3 s_partrot;

out vec2 uv; 
out float brig;
out vec3 worldPos;

const float PI = 3.14159265358979323846;

mat4 rotateX(mat4 m, float a)
{
    float c = cos(a);
    float s = sin(a);

    mat4 R = mat4(
        1,0,0,0,
        0,c,s,0,
        0,-s,c,0,
        0,0,0,1
    );

    return m * R;
}

mat4 rotateY(mat4 m, float a)
{
    float c = cos(a);
    float s = sin(a);

    mat4 R = mat4(
         c,0,-s,0,
         0,1, 0,0,
         s,0, c,0,
         0,0, 0,1
    );

    return m * R;
}

mat4 rotateZ(mat4 m, float a)
{
    float c = cos(a);
    float s = sin(a);

    mat4 R = mat4(
         c,s,0,0,
        -s,c,0,0,
         0,0,1,0,
         0,0,0,1
    );

    return m * R;
}

mat4 translate(mat4 m, vec3 p)
{
    mat4 T = mat4(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        p.x,p.y,p.z,1
    );

    return m * T;
}

mat4 scale(mat4 m, vec3 s)
{
    mat4 S = mat4(
        s.x,0,0,0,
        0,s.y,0,0,
        0,0,s.z,0,
        0,0,0,1
    );

    return m * S;
}

void main()
{
    mat4 model = mat4(1.0);
    float t = s_seconds * 10.0 * iSpeed + iTimeOff;
    float dy = -abs(sin(0.6662 * t)) * 5.0 - 23.0;
    model = translate(model, iPos);
    model = scale(model, vec3(s_scale, -s_scale, s_scale));
    model = translate(model, vec3(0.0, dy, 0.0));
    model = rotateY(model, iRot);
    
    vec3 partrot;
    switch (s_partindex) {
    case 0:
        partrot = vec3(0.8 * sin(t), sin(0.83 * t), 0.0);
        break;
    case 1:
        partrot = vec3(0.0, 0.0, 0.0);
        break;
    case 2:
        partrot = vec3(sin(0.6662 * t + PI) * 2.0, 0.0, sin(0.2312 * t + 1.0));
        break;
    case 3:
        partrot = vec3(sin(0.6662 * t) * 2.0, 0.0, sin(0.2812 * t) - 1.0);
        break;
    case 4:
        partrot = vec3(sin(0.6662 * t) * 1.4, 0.0, 0.0);
        break;
    case 5:
        partrot = vec3(sin(0.6662 * t + PI) * 1.4, 0.0, 0.0);
        break;
    }

    mat4 local = mat4(1.0);
    local = translate(local, s_partrot);
    local = rotateX(local, partrot.z);
    local = rotateY(local, partrot.y);
    local = rotateZ(local, partrot.x);
    
    vec4 world = model * local * vec4(aPos, 1.0);
    worldPos = world.xyz;
    brig = iBrig;
    uv = aUV;
    gl_Position = s_VP * world;
}

#shader fragment
#version 330 core

in vec2 uv;
in vec3 worldPos;
in float brig;

out vec4 FragColor;

uniform sampler2D s_t1;

uniform vec3 s_cpos;
uniform vec4 s_fcolor0;
uniform float s_fdensity0;
uniform vec4 s_fcolor1;
uniform float s_fdensity1;

void main()
{
    vec4 tex = texture(s_t1, uv);
    vec4 result = tex * brig;
    float dist = length(worldPos - s_cpos);
    vec3 finalColor;
    if (brig < 0.6) {
        float fog = exp(-s_fdensity1 * dist);
        finalColor = mix(s_fcolor0.rgb, result.rgb, fog);
    }
    else {
        float fog = exp(-s_fdensity0 * dist);
        finalColor = mix(s_fcolor1.rgb, result.rgb, fog);
    }

    FragColor = vec4(finalColor, result.a);
}