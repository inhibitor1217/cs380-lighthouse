#version 330 core

in float fog;
in vec3 camera_direction;
in vec3 world_normal;

out vec4 out_color;

uniform vec3 color;
uniform vec3 fog_color = vec3(0.5, 0.5, 0.5);

const vec3 light_direction = vec3(1, 1, 1);
const vec3 light_color = vec3(1, 1, 1);

void main()
{
	vec3 n_world_normal = normalize(world_normal);
	vec3 n_light_direction = normalize(light_direction);
	vec3 n_camera_direction = normalize(camera_direction);

    float diffuse_factor = clamp( dot(n_world_normal, n_light_direction), 0, 1);

	vec3 reflection = 2.0 * dot(n_world_normal, n_light_direction) * n_world_normal - n_light_direction;
	float specular_factor = pow(dot(reflection, n_camera_direction), 30.0);

	vec4 result_color = vec4(diffuse_factor * color + specular_factor * light_color, 0.5);
    out_color = mix(result_color, vec4(fog_color, 1), fog);
}
