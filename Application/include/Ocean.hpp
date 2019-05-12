#pragma once

#include "RenderObject.hpp"

class Ocean : public Engine::RenderObject {

public:
	Ocean();
	void Animate(float deltaTime);

private:
	Engine::Mesh *__GenerateMesh();
};