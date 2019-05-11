#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;
out vec4 output_color;

struct Light
{
	int type;
	vec3 diffuse_illuminance;
	vec3 pos;
};

uniform mat4 cameraTransform;
uniform int numLights;
uniform Light lights[3];

uniform vec3 diffuse_reflectance;

void main()
{
	vec3 intensity = vec3(0);
	vec3 normal = normalize(fragmentNormal.xyz);

	mat4 worldToCamera = inverse(cameraTransform);
	for (int i=0; i<numLights; i++)
	{
		vec4 pos = worldToCamera * vec4(lights[i].pos, 1);
		vec3 tolight = normalize(pos.xyz - fragmentPosition.xyz);
		float diffuse = max(0, dot(normal, tolight));
		
		intensity += diffuse_reflectance * lights[i].diffuse_illuminance * diffuse;
	}

	output_color = vec4(intensity, 1);
	output_color.rgb = pow(output_color.rgb, vec3(1.0/2.2));

	
}
