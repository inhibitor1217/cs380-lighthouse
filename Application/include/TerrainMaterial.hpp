#pragma once

#include <Material.hpp>

class TerrainMaterial : public Engine::Material
{
public:
	void CreateMaterial();
	void UpdateColor(glm::vec3 color);
	void UpdateFogColor(glm::vec3 color);
	void UpdateTime(float time);
	void UpdateCameraPos(glm::vec3 cameraPosition);
};