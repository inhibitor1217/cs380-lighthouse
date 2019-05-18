#include "TerrainMaterial.hpp"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BUF_SIZE 50

void TerrainMaterial::CreateMaterial()
{
	_program->AttachShader("Resources\\Materials\\TerrainVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\BlinnPhongFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();
}

void TerrainMaterial::UpdateAmbientReflectance(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "ambient_reflectance");
	glUniform3fv(uniformLocation, 1, (float *)&color);
}

void TerrainMaterial::UpdateDiffuseReflectance(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
	glUniform3fv(uniformLocation, 1, (float *)&color);
}

void TerrainMaterial::UpdateSpecularReflectance(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "specular_reflectance");
	glUniform3fv(uniformLocation, 1, (float *)&color);
}

void TerrainMaterial::UpdateFogColor(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint uniformLocation = glGetUniformLocation(_program->GetProgramId(), "fog_color");
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

void TerrainMaterial::UpdateLight(std::vector<Light> &lights)
{
	GLuint pid = _program->GetProgramId();
	glUseProgram(pid);

	int numLights = 0;
	GLuint location;

	char buf[BUF_SIZE];
	for (int i = 0; i < lights.size(); i++)
	{
		if (lights[i].enabled)
		{
			snprintf(buf, BUF_SIZE, "lights[%d].type", numLights);
			location = glGetUniformLocation(pid, buf);
			glUniform1i(location, lights[i].type);

			snprintf(buf, BUF_SIZE, "lights[%d].diffuse_illuminance", numLights);
			location = glGetUniformLocation(pid, buf);
			glUniform3fv(location, 1, (float*)&(lights[i].diffuse_illuminance));

			snprintf(buf, BUF_SIZE, "lights[%d].specular_illuminance", numLights);
			location = glGetUniformLocation(pid, buf);
			glUniform3fv(location, 1, (float*)&(lights[i].specular_illuminance));

			snprintf(buf, BUF_SIZE, "lights[%d].light_direction", numLights);
			location = glGetUniformLocation(pid, buf);
			glUniform3fv(location, 1, (float*)&(lights[i].light_direction));

			glm::mat4 world_transform = lights[i].transform.GetWorldTransform();
			glm::vec4 local_pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
			glm::vec4 world_pos = world_transform * local_pos;
			snprintf(buf, BUF_SIZE, "lights[%d].pos", numLights);
			location = glGetUniformLocation(pid, buf);
			glUniform3fv(location, 1, (float*)&(world_pos));

			numLights++;
			if (numLights >= MAX_LIGHTS)
				break;
		}
	}

	location = glGetUniformLocation(pid, "numLights");
	glUniform1i(location, numLights);
}