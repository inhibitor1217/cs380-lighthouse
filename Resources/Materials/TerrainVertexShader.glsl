#version 330 core

layout(location = 0) in vec3 pos;

out vec4 fragmentPosition;
out vec4 fragmentNormal;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

uniform float time;
uniform vec3 cameraPosition;

const float g = 2.00;

const vec4 wave[] = {
    vec4( 1.20, -1.53, 0.05, 1.0),
	vec4( 1.30, -4.80, 0.02, 1.0),
	vec4(-1.70, -3.30, 0.04, 1.0),
	vec4(-0.50, -0.35, 0.08, 1.0),
	vec4(-0.29,  0.71, 0.05, 1.0),
	vec4( 0.17, -0.35, 0.06, 1.0),
};
const int numWaves = 6;

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[0][3] = 0;
	invm[1][3] = 0;
	invm[2][3] = 0;

	return transpose(invm);
}

void main()
{
	vec2 worldPos = (worldTransform * vec4(pos, 1)).xy;
	vec3 pos_offset = vec3(0, 0, 0);
	vec3 pos_dx = vec3(0, 0, 0);
	vec3 pos_dy = vec3(0, 0, 0);
	
	for (int i = 0; i < numWaves; i++)
	{
		vec2 wave_vector = wave[i].xy;
		float k = length(wave_vector);
		float wave_steepness = 1 / (numWaves * k * k);
		float phase = dot(wave_vector, worldPos) - sqrt(g * k) * time;
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
	
    gl_Position = projectionMatrix * inverse(cameraTransform) * worldTransform * vec4(pos + pos_offset, 1);
	
	mat4 MVM = inverse(cameraTransform) * worldTransform;
	mat4 NVM = NormalMatrix(MVM);

	vec4 wPosition = MVM * vec4(pos + pos_offset, 1);
	fragmentPosition = wPosition;
	fragmentNormal = NVM * vec4(cross(pos_dx, pos_dy), 0);
	gl_Position = projectionMatrix * wPosition;
}
