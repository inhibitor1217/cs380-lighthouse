#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;

out vec4 out_color;

struct Light
{
	int type;
	vec3 diffuse_illuminance;
	vec3 specular_illuminance;
	vec3 pos;
	vec3 light_direction;
};

uniform mat4 cameraTransform;
uniform int numLights;
uniform Light lights[3];

uniform vec3 diffuse_reflectance;

uniform vec3 fog_color = vec3(0.5, 0.5, 0.5);

void main()
{
	vec3 intensity = vec3(0);
	vec3 n_normal = normalize(fragmentNormal.xyz);
	vec3 n_light_direction;
	
	mat4 worldToCamera = inverse(cameraTransform);
	for (int i = 0; i < numLights; i++)
	{
		if (lights[i].type == 1)
		{
			// Point Light
		}
		else if(lights[i].type == 2)
		{
			// Directional Light
			n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);
		}
		else if(lights[i].type == 3)
		{
			// Spotlight
		}

		float diffuse_factor  = clamp( dot(n_normal, n_light_direction), 0, 1);
		vec3 n_half_angle     = normalize( n_light_direction - normalize(fragmentPosition.xyz) );
		float specular_factor = pow( dot(n_normal, n_half_angle), 50.0);

		intensity += diffuse_factor * lights[i].diffuse_illuminance * diffuse_reflectance + specular_factor * lights[i].specular_illuminance;
	}

	out_color = vec4(intensity, 0.5);

	float fog = clamp( 0.033 * length(fragmentPosition) - 1.0 , 0, 1);
    out_color = mix(out_color, vec4(fog_color, 1), fog);
}
