
#include "Ocean.hpp"
#include "TerrainMaterial.hpp"

#include "Geometry.hpp"

constexpr float PI = 3.14159265;

Ocean::Ocean() : Engine::RenderObject()
{
	// Engine::Mesh *terrainMesh = __GenerateMesh();
	// SetMesh(terrainMesh);
	Geometry *geometry = new Geometry();
	Engine::Mesh *mesh = new Engine::Mesh();
	geometry->GenerateCube(mesh);
	SetMesh(mesh);
	
	TerrainMaterial *material = new TerrainMaterial();
	material->CreateMaterial();
	SetMaterial(material);
}

void Ocean::Animate(float deltaTime)
{
	static float angle = 0.0f;
	angle += deltaTime;

	_transform.SetOrientation(glm::rotate(
		glm::mat4(1.0f),
		angle,
		glm::vec3(1, 1, 1)
	));
}

Engine::Mesh *Ocean::__GenerateMesh()
{
	Engine::Mesh *mesh = new Engine::Mesh();

	mesh->AddAttribute(3);
	mesh->AddAttribute(3);

	glm::vec3 position_data[] = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f,1.0f, 0.0f)
	};
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 3; i++)
	{
		mesh->AddVertexData(position_data[i]);
		mesh->AddVertexData(normal);
	}
	mesh->SetNumElements(3);
	mesh->CreateMesh();

	return mesh;
}