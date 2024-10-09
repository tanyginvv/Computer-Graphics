uniform float u_time;

void main()
{
    vec4 position = gl_Vertex;
    float x = position.x;
    float y = position.y;
    float z1 = x * x / 3 + y * y / 2;
    float z2 = x * x / 3 - y * y / 2;
    position.z = mix(z1, z2, u_time);

    gl_Position = gl_ModelViewProjectionMatrix * position;
}