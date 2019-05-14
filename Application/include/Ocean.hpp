#pragma once

#include "RenderObject.hpp"
#include "TerrainMaterial.hpp"

class Ocean {

public:
	Ocean(float _CHUNK_SIZE, int _TERRAIN_SIZE, float _LOD, float center_z);
	~Ocean();
	void Update(float time, glm::vec3 cameraPosition, glm::vec3 color, glm::vec3 fogColor);
	void Render(Engine::Camera *camera, float angle);

private:
	float CHUNK_SIZE;
	int TERRAIN_SIZE;
	float LOD;
	std::vector<Engine::RenderObject *> __objects;
	TerrainMaterial *__material;

	void generateObjects(float center_z);
	Engine::RenderObject *Ocean::generateChunk(float center_x, float center_y, float center_z, int lod);
};