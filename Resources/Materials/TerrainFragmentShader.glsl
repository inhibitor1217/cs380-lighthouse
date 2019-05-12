#version 330 core

in vec3 world_normal;

out vec4 out_color;

uniform vec3 color;

const vec3 light_direction = vec3(0, 0, 1);

void main()
{
    float diffuse_factor = clamp( dot(world_normal, light_direction), 0, 1);
    out_color = vec4(diffuse_factor * color, 1);
}
