#include "TerrainMaterial.hpp"

void TerrainMaterial::CreateMaterial()
{
	_program->AttachShader("Resources\\Materials\\TerrainVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\TerrainFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();

	glm::vec3 initialColor = glm::vec3((float)0x8B / 255.0f, (float)0xD9 / 255.0f, (float)0xC7 / 255.0f);

	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "color");
	glUniform3fv(uniformLocation, 1, (float *)&initialColor);
}

void TerrainMaterial::UpdateColor(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "color");
	glUniform3fv(uniformLocation, 1, (float *)&color);
}