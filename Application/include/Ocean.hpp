#pragma once

#include "RenderObject.hpp"

class Ocean : public Engine::RenderObject {

public:
	Ocean(float _CHUNK_SIZE, float _TERRAIN_SIZE, float _LOD);
	void Update(float time, glm::vec3 cameraPosition, glm::vec3 color, glm::vec3 fogColor);

private:
	float CHUNK_SIZE;
	float TERRAIN_SIZE;
	float LOD;

	Engine::Mesh *__GenerateMesh();
	int generateChunk(Engine::Mesh *mesh, float center_x, float center_y, int lod, int start_index, int &numVertices);
};