#pragma once

#include "RenderObject.hpp"

class Ocean : public Engine::RenderObject {

public:
	Ocean();
	void Animate(float time, glm::vec3 cameraPosition);

private:
	Engine::Mesh *__GenerateMesh();
};