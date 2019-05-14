#version 330 core

in vec3 world_normal;

out vec4 out_color;

uniform vec3 color;

const vec3 light_direction = vec3(1, 1, 1);

void main()
{
	vec3 n_world_normal = normalize(world_normal);
	vec3 n_light_direction = normalize(light_direction);

    float diffuse_factor = clamp( dot(n_world_normal, n_light_direction), 0, 1);

	out_color = vec4(diffuse_factor * color, 1);
}
