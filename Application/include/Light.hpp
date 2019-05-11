#pragma once

#include <glm/glm.hpp>

#include <Transform.hpp>

#define MAX_LIGHTS 3
// Feel free to increase maximum number of lights!

enum LightType : int
{
	PointLight = 1,
	DirectionalLight = 2,
	Spotlight = 3,
};

struct Light
{
	// TODO: support spotlight and (point light or directional light)
	// Note - you also need to modify struct Light in .glsl files!
	LightType type;
	int enabled = 1;		// Your light should be able to turn on/off independently!

	Engine::Transform transform;
	glm::vec3 diffuse_illuminance;
};
