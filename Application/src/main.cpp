// STL
#include <iostream>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Ocean.hpp>
#include <DiffuseMaterial.hpp>
#include <PhongMaterial.hpp>

// include Engine
#include "ModelLoader.hpp"

GLFWwindow* g_window;
float g_window_width = 1440.0f;
float g_window_height = 1080.0f;
int g_framebuffer_width = 1440;
int g_framebuffer_height = 1080;

Engine::Camera *mainCamera;

static bool mode_wireframe = false;
static bool mode_cam_left = false;
static bool mode_cam_right = false;

static bool mode_render_top = true;
static bool mode_render_bottom = true;

constexpr float PI = 3.1415926f;
constexpr float CAMERA_ROTATE_SPEED = 0.5f;

static float camera_direction = 0.0f;

glm::vec3 genColor(unsigned int color)
{
	return glm::vec3((float)((color & 0xFF0000) >> 16) / 255.0f, (float)((color & 0xFF00) >> 8) / 255.0f, (float)(color & 0xFF) / 255.0f);
}

static auto ocean_color        = genColor(0x2A93D5);
static auto ocean_sub_color    = genColor(0x135589);
static auto sky_color          = genColor(0xEDFAFD);
static auto island_color       = genColor(0x95571F);
static auto lighthouse_color_1 = genColor(0xC54021);
static auto lighthouse_color_2 = genColor(0xC9C9C9);

static std::vector<Light> lights;

static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	
}

static void CursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{

}

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	if (a_action == GLFW_PRESS)
	{
		switch (a_key)
		{
		case GLFW_KEY_Z:
			mode_wireframe = !mode_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, mode_wireframe ? GL_LINE : GL_FILL);
			if (mode_wireframe)
			{
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
			}
			else
			{
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
			}
			break;
		case GLFW_KEY_1:
			mode_render_top = !mode_render_top;
			break;
		case GLFW_KEY_2:
			mode_render_bottom = !mode_render_bottom;
			break;
		case GLFW_KEY_LEFT:
			mode_cam_left = true;
			break;
		case GLFW_KEY_RIGHT:
			mode_cam_right = true;
			break;
		}
	}
	else if (a_action == GLFW_RELEASE)
	{
		switch (a_key)
		{
		case GLFW_KEY_LEFT:
			mode_cam_left = false;
			break;
		case GLFW_KEY_RIGHT:
			mode_cam_right = false;
		}
	}
}

int main(int argc, char** argv)
{
	// Initialize GLFW library
	if (!glfwInit())
	{
		return -1;
	}

	// Create window and OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, false);

	/* Create a windowed mode window and its OpenGL context */
	g_window = glfwCreateWindow(g_window_width, g_window_height, "Homework 3 - 20170737", NULL, NULL);
	if (!g_window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(g_window);

	// Initialize GLEW library
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
		exit(1);
	}

	Ocean *oceanTerrain = new Ocean(16, 3, 8, 0.0f);
	Ocean *oceanSubsurface = new Ocean(16, 3, 6, -0.5f);

	mainCamera = new Engine::Camera(
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::rotate(
			glm::rotate(glm::mat4(1.0f), 0.5f * PI, glm::vec3(1, 0, 0)),
			camera_direction, glm::vec3(0, 1, 0)
		)
	);
	mainCamera->SetProjection(g_window_width / g_window_height, 70.0, 0.1, 120);

	Engine::ModelLoader *loader = new Engine::ModelLoader("Resources\\Models\\lighthouse.obj");

	Engine::Mesh *lighthouseIslandMesh = new Engine::Mesh();
	loader->GenerateMesh(lighthouseIslandMesh, 0);
	Engine::Mesh *lighthouseMesh1 = new Engine::Mesh();
	loader->GenerateMesh(lighthouseMesh1, 1);
	Engine::Mesh *lighthouseMesh2 = new Engine::Mesh();
	loader->GenerateMesh(lighthouseMesh2, 2);

	DiffuseMaterial *diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->CreateMaterial();

	Engine::RenderObject *lighthouseIslandObj = new Engine::RenderObject(lighthouseIslandMesh, diffuseMaterial);
	lighthouseIslandObj->GetTransform()->SetPosition(glm::vec3(30, -20, 0));
	Engine::RenderObject *lighthouseObj1 = new Engine::RenderObject(lighthouseMesh1, diffuseMaterial);
	lighthouseObj1->GetTransform()->AddParent(lighthouseIslandObj->GetTransform());
	Engine::RenderObject *lighthouseObj2 = new Engine::RenderObject(lighthouseMesh2, diffuseMaterial);
	lighthouseObj2->GetTransform()->AddParent(lighthouseIslandObj->GetTransform());
	
	lights.push_back(Light());
	lights[0].type = DirectionalLight;
	lights[0].enabled = true;
	lights[0].light_direction = glm::vec3(1, 1, 1);
	lights[0].diffuse_illuminance = sky_color;
	lights[0].specular_illuminance = sky_color;

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	// glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
	// glfwSetCursorPosCallback(g_window, CursorPosCallback);
	glfwSetKeyCallback(g_window, KeyboardCallback);

	float prev_time = (float)glfwGetTime();
	float avg_elapsed_time = 1.0f / 60.0f;

	glClearColor(sky_color.x, sky_color.y, sky_color.z, 1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		avg_elapsed_time = avg_elapsed_time * (59.0 / 60.0) + elapsed_time * (1.0 / 60.0);
		prev_time = total_time;

		/* Clear buffers. */
		glClear(GL_COLOR_BUFFER_BIT | (mode_wireframe ? 0 : GL_DEPTH_BUFFER_BIT));

		/* Animate Objects. */
		oceanTerrain->Update(
			total_time, 
			mainCamera->GetTransform()->GetPosition(),
			ocean_color,
			sky_color,
			lights
		);
		oceanSubsurface->Update(
			0.7f * total_time, 
			mainCamera->GetTransform()->GetPosition(),
			ocean_sub_color,
			sky_color,
			lights
		);

		if (mode_cam_left)
			camera_direction += elapsed_time * CAMERA_ROTATE_SPEED;
		if (mode_cam_right)
			camera_direction -= elapsed_time * CAMERA_ROTATE_SPEED;

		mainCamera->GetTransform()->SetOrientation(
			glm::rotate(
				glm::rotate(glm::mat4(1.0f), 0.5f * PI, glm::vec3(1, 0, 0)), 
				camera_direction, glm::vec3(0, 1, 0)
			)
		);

		/* Render pass. */

		glEnable(GL_BLEND);
		if (mode_render_bottom)
			oceanSubsurface->Render(mainCamera, camera_direction + 0.5f * PI);
		if (mode_render_top)
			oceanTerrain->Render(mainCamera, camera_direction + 0.5f * PI);

		diffuseMaterial->UpdateLight(lights);

		diffuseMaterial->UpdateDiffuseReflectance(island_color);
		lighthouseIslandObj->Render(mainCamera);
		diffuseMaterial->UpdateDiffuseReflectance(lighthouse_color_1);
		lighthouseObj1->Render(mainCamera);
		diffuseMaterial->UpdateDiffuseReflectance(lighthouse_color_2);
		lighthouseObj2->Render(mainCamera);

		glDisable(GL_BLEND);


		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}