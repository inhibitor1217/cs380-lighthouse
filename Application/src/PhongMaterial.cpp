#include <PhongMaterial.hpp>

void PhongMaterial::CreateMaterial()
{
	_program->AttachShader("Resources\\Materials\\PhongVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\PhongFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();
}

void PhongMaterial::UpdateAmbientReflectance(glm::vec3 color)
{
    // TODO
}

void PhongMaterial::UpdateDiffuseReflectance(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "color");
	glUniform3fv(uniformLocation, 1, (float *)&color);
}

void PhongMaterial::UpdateSpecularReflectance(glm::vec3 color)
{
	// TODO
}

void PhongMaterial::UpdateLight(std::vector<Light> &lights)
{
    // TODO
}
