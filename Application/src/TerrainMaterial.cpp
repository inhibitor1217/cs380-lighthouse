#include "TerrainMaterial.hpp"

void TerrainMaterial::CreateMaterial()
{
	_program->AttachShader("Resources\\Materials\\TerrainVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\TerrainFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();

	glm::vec3 initialColor = glm::vec3((float)0x13 / 255.0f, (float)0x55 / 255.0f, (float)0x89 / 255.0f);

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

void TerrainMaterial::UpdateTime(float time)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "time");
	glUniform1f(uniformLocation, time);
}

void TerrainMaterial::UpdateCameraPos(glm::vec3 cameraPosition)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "cameraPosition");
	glUniform3fv(uniformLocation, 1, (float *)&cameraPosition);
}