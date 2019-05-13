#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

uniform float time;
uniform vec3 cameraPosition;

const float g = 2.00;

const vec4 wave[] = {
    vec4( 1.20, -1.53, 0.05, 1.0),
	vec4( 3.30, -4.80, 0.04, 1.0),
	vec4(-1.70, -1.30, 0.07, 1.0),
	vec4( 3.50, -0.10, 0.03, 1.0),
};
const int numWaves = 4;

out vec3 camera_direction;
out vec3 world_normal;

void main()
{
	vec3 pos_offset = vec3(0, 0, 0);
	vec3 pos_dx = vec3(0, 0, 0);
	vec3 pos_dy = vec3(0, 0, 0);

	for (int i = 0; i < numWaves; i++)
	{
		vec2 wave_vector = wave[i].xy;
		float k = length(wave_vector);
		float wave_steepness = 1 / (numWaves * pow(k, 3));
		float phase = dot(wave_vector, pos.xy) - sqrt(g * k) * time;
		pos_offset += wave[i].w * vec3(
			wave_steepness * cos(phase) * wave_vector,
			wave[i].z * sin(phase)
		);
		pos_dx += wave[i].w * vec3(
		    (1.0 / numWaves) - wave_steepness * wave_vector.x * wave_vector.x * sin(phase),
			                 - wave_steepness * wave_vector.x * wave_vector.y * sin(phase),
			wave[i].z * wave_vector.x * cos(phase)
		);
		pos_dy += wave[i].w * vec3(
			                 - wave_steepness * wave_vector.x * wave_vector.y * sin(phase),
			(1.0 / numWaves) - wave_steepness * wave_vector.y * wave_vector.y * sin(phase),
			wave[i].z * wave_vector.y * cos(phase)
		);
	}

    gl_Position = projectionMatrix * inverse(cameraTransform) * vec4(pos + pos_offset, 1);

	camera_direction = cameraPosition - (pos + pos_offset);
	world_normal = normalize(cross(pos_dx, pos_dy));
}
