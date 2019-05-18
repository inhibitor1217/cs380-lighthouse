#pragma once

#include <Material.hpp>
#include <Light.hpp>

class TerrainMaterial : public Engine::Material
{
public:
	void CreateMaterial();
	void UpdateAmbientReflectance(glm::vec3 color);
	void UpdateDiffuseReflectance(glm::vec3 color);
	void UpdateSpecularReflectance(glm::vec3 color);
	void UpdateFogColor(glm::vec3 color);
	void UpdateTime(float time);
	void UpdateCameraPos(glm::vec3 cameraPosition);
	void UpdateLight(std::vector<Light> &lights);
};