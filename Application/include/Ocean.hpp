#pragma once

#include "RenderObject.hpp"
#include "TerrainMaterial.hpp"

constexpr int NUM_WAVES = 4;
constexpr float WAVE_VECTOR_LENGTH_MIN = 0.7;
constexpr float WAVE_VECTOR_LENGTH_MAX = 5.0;

class Ocean {

public:
	Ocean(float _CHUNK_SIZE, int _TERRAIN_SIZE, float _LOD, float center_z);
	~Ocean();
	void SetMaterial(Engine::Material *material);
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