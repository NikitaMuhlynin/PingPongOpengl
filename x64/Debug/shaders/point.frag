#version 330 core

in float vAlpha;

uniform vec3 uColor;

out vec4 FragColor;

void main()
{
    vec2 p = gl_PointCoord * 2.0 - 1.0;
    float r2 = dot(p, p);

    if (r2 > 1.0)
        discard;

    float softEdge = 1.0 - smoothstep(0.72, 1.0, r2);
    FragColor = vec4(uColor, vAlpha * softEdge);
}