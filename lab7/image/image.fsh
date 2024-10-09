bool PointIsOnTheLeft(vec2 p0, vec2 p1, vec2 p)
{
    vec2 p0p1 = p1 - p0;
    vec2 n = vec2(-p0p1.y, p0p1.x);
    
    return dot(p - p0, n) > 0.0;
}

bool PointIsInsideStar(
    vec2 p0, vec2 p1, vec2 p2,
    vec2 p3, vec2 p4, vec2 p5,
    vec2 p6, vec2 p7, vec2 p8,
    vec2 p9, vec2 p)
{
    return
        (PointIsOnTheLeft(p1, p0, p) && PointIsOnTheLeft(p0, p2, p) && PointIsOnTheLeft(p2, p1, p)) ||
        (PointIsOnTheLeft(p2, p3, p) && PointIsOnTheLeft(p3, p4, p) && PointIsOnTheLeft(p4, p2, p)) ||
        (PointIsOnTheLeft(p6, p4, p) && PointIsOnTheLeft(p4, p5, p) && PointIsOnTheLeft(p5, p6, p)) ||
        (PointIsOnTheLeft(p8, p6, p) && PointIsOnTheLeft(p6, p7, p) && PointIsOnTheLeft(p7, p8, p)) ||
        (PointIsOnTheLeft(p9, p1, p) && PointIsOnTheLeft(p1, p8, p) && PointIsOnTheLeft(p8, p9, p)) ||
        (PointIsOnTheLeft(p1, p2, p) && PointIsOnTheLeft(p2, p4, p) && PointIsOnTheLeft(p4, p6, p) &&
         PointIsOnTheLeft(p6, p8, p) && PointIsOnTheLeft(p8, p1, p));
}

void main()
{
    vec2 pos = gl_TexCoord[0].xy;

    const vec2 p0 = vec2(1.99, 3.81);
    const vec2 p1 = vec2(2.43, 2.43);
    const vec2 p2 = vec2(1.55, 2.42);
    const vec2 p3 = vec2(0.15, 2.39);
    const vec2 p4 = vec2(1.25, 1.55);
    const vec2 p5 = vec2(0.85, 0.16);
    const vec2 p6 = vec2(1.99, 0.97);
    const vec2 p7 = vec2(3.11, 0.18);
    const vec2 p8 = vec2(2.72, 1.55);
    const vec2 p9 = vec2(3.80, 2.39);

    if (PointIsInsideStar(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, pos))
    {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else
    {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }

    const vec2 p0 = vec2();     

    if (PointIsInsideStar())
    {
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else
    {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }

}