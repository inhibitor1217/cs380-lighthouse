
#include <algorithm>

#include "Ocean.hpp"
#include "TerrainMaterial.hpp"

#include "Geometry.hpp"

constexpr float PI = 3.14159265;
constexpr float CHUNK_SIZE = 8;
constexpr int TERRAIN_SIZE = 0;
constexpr float LOD = 7;

Ocean::Ocean() : Engine::RenderObject()
{
	Engine::Mesh *terrainMesh = __GenerateMesh();
	SetMesh(terrainMesh);
	
	TerrainMaterial *material = new TerrainMaterial();
	material->CreateMaterial();
	SetMaterial(material);
}

void Ocean::Animate(float time, glm::vec3 cameraPosition)
{
	TerrainMaterial *terrainMaterial = reinterpret_cast<TerrainMaterial *>(_material);
	terrainMaterial->UpdateTime(time);
	terrainMaterial->UpdateCameraPos(cameraPosition);
}

int generateChunk(Engine::Mesh *mesh, float center_x, float center_y, int lod, int start_index, int &numVertices)
{
	int num_grid = (1 << lod);
	int ret = 0;

	for (int i_x = 0; i_x <= num_grid; i_x++)
		for (int i_y = 0; i_y <= num_grid; i_y++)
		{
			mesh->AddVertexData(glm::vec3(
				center_x + CHUNK_SIZE * ((float)i_x / (float)num_grid - 0.5f),
				center_y + CHUNK_SIZE * ((float)i_y / (float)num_grid - 0.5f),
				0.0f
			));
			numVertices++;

			int idx = i_x * (num_grid + 1) + i_y;
			if (i_x < num_grid && i_y < num_grid)
			{
				if ((i_x + i_y) & 1)
				{
					mesh->AddIndexData(start_index + idx);
					mesh->AddIndexData(start_index + idx + num_grid + 1);
					mesh->AddIndexData(start_index + idx + 1);
					mesh->AddIndexData(start_index + idx + 1);
					mesh->AddIndexData(start_index + idx + num_grid + 1);
					mesh->AddIndexData(start_index + idx + num_grid + 2);
				}
				else
				{
					mesh->AddIndexData(start_index + idx);
					mesh->AddIndexData(start_index + idx + num_grid + 1);
					mesh->AddIndexData(start_index + idx + num_grid + 2);
					mesh->AddIndexData(start_index + idx);
					mesh->AddIndexData(start_index + idx + num_grid + 2);
					mesh->AddIndexData(start_index + idx + 1);
				}
				ret += 6;
			}
		}

	return ret;
}

Engine::Mesh *Ocean::__GenerateMesh()
{
	Engine::Mesh *mesh = new Engine::Mesh();

	mesh->SetHasIndex(true);
	mesh->AddAttribute(3);

	int numElements = 0, numVertices = 0;
	for (int cx = -TERRAIN_SIZE; cx <= TERRAIN_SIZE; cx++)
		for (int cy = -TERRAIN_SIZE; cy <= TERRAIN_SIZE; cy++)
		{
			numElements += generateChunk(
				mesh, cx * CHUNK_SIZE, cy * CHUNK_SIZE,
				LOD,
				numVertices, numVertices
			);
		}

	mesh->SetNumElements(numElements);
	mesh->CreateMesh();

	return mesh;
}