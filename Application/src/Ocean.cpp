
#include <algorithm>

#include "Ocean.hpp"

constexpr float PI = 3.14159265;

Ocean::Ocean(float _CHUNK_SIZE, int _TERRAIN_SIZE, float _LOD, float center_z)
{
	CHUNK_SIZE = _CHUNK_SIZE;
	TERRAIN_SIZE = _TERRAIN_SIZE;
	LOD = _LOD;

	__objects = std::vector<Engine::RenderObject *>();
	
	__material = new TerrainMaterial();
	__material->CreateMaterial();
	
	generateObjects(center_z);
}

Ocean::~Ocean()
{
	for (auto obj : __objects)
		delete obj;
}

void Ocean::Update(float time, glm::vec3 cameraPosition, glm::vec3 color, glm::vec3 fogColor, std::vector<Light> &lights)
{
	__material->UpdateTime(time);
	__material->UpdateCameraPos(cameraPosition);
	__material->UpdateDiffuseReflectance(color);
	__material->UpdateFogColor(fogColor);
	__material->UpdateLight(lights);
}

void Ocean::Render(Engine::Camera *camera, float angle)
{
	for (int idx = 0; idx < __objects.size(); idx++)
	{
		int cx = idx / (2 * TERRAIN_SIZE + 1) - TERRAIN_SIZE;
		int cy = idx % (2 * TERRAIN_SIZE + 1) - TERRAIN_SIZE;

		glm::vec2 camera_direction = glm::vec2(cos(angle), sin(angle));
		glm::vec2 chunk_direction = glm::vec2(cx, cy);

		if (cx == 0 && cy == 0)
			__objects[idx]->Render(camera);
		else if (glm::dot(glm::normalize(camera_direction), glm::normalize(chunk_direction)) > 0.0f)
			__objects[idx]->Render(camera);

	}
}

Engine::RenderObject *Ocean::generateChunk(float center_x, float center_y, float center_z, int lod)
{
	int num_grid = (1 << lod);
	int ret = 0;

	Engine::Mesh *mesh = new Engine::Mesh();

	mesh->SetHasIndex(true);
	mesh->AddAttribute(3);

	for (int i_x = 0; i_x <= num_grid; i_x++)
		for (int i_y = 0; i_y <= num_grid; i_y++)
		{
			mesh->AddVertexData(glm::vec3(
				center_x + CHUNK_SIZE * ((float)i_x / (float)num_grid - 0.5f),
				center_y + CHUNK_SIZE * ((float)i_y / (float)num_grid - 0.5f),
				center_z
			));
			
			int idx = i_x * (num_grid + 1) + i_y;
			if (i_x < num_grid && i_y < num_grid)
			{
				if ((i_x + i_y) & 1)
				{
					mesh->AddIndexData(idx);
					mesh->AddIndexData(idx + num_grid + 1);
					mesh->AddIndexData(idx + 1);
					mesh->AddIndexData(idx + 1);
					mesh->AddIndexData(idx + num_grid + 1);
					mesh->AddIndexData(idx + num_grid + 2);
				}
				else
				{
					mesh->AddIndexData(idx);
					mesh->AddIndexData(idx + num_grid + 1);
					mesh->AddIndexData(idx + num_grid + 2);
					mesh->AddIndexData(idx);
					mesh->AddIndexData(idx + num_grid + 2);
					mesh->AddIndexData(idx + 1);
				}
				ret += 6;
			}
		}

	mesh->SetNumElements(ret);
	mesh->CreateMesh();

	return new Engine::RenderObject(mesh, __material);
}

void Ocean::generateObjects(float center_z)
{
	for (int cx = -TERRAIN_SIZE; cx <= TERRAIN_SIZE; cx++)
		for (int cy = -TERRAIN_SIZE; cy <= TERRAIN_SIZE; cy++)
		{
			__objects.push_back(
				generateChunk(cx * CHUNK_SIZE, cy * CHUNK_SIZE, center_z, LOD - std::max(std::abs(cx), std::abs(cy)))
			);
		}
}