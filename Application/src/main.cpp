// STL
#include <iostream>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Ocean.hpp>

// include Engine



GLFWwindow* g_window;
float g_window_width = 1920;
float g_window_height = 1440.0f;
int g_framebuffer_width = 1920;
int g_framebuffer_height = 1440;

Engine::Camera *mainCamera;

static bool mode_wireframe = false;
static bool mode_top_view = false;
static bool mode_cam_left = false;
static bool mode_cam_right = false;

constexpr float PI = 3.1415926f;
constexpr float CAMERA_ROTATE_SPEED = 0.5f;

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
		case GLFW_KEY_F1:
			mode_top_view = !mode_top_view;
			if (mode_top_view)
			{
				mainCamera->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 60.0f));
				mainCamera->GetTransform()->SetOrientation(glm::mat4(1.0f));
			}
			else
			{
				mainCamera->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
				mainCamera->GetTransform()->SetOrientation(glm::rotate(
					glm::mat4(1.0f), 0.5f * PI, glm::vec3(1.0f, 0.0f, 0.0f))
				);
			}
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

	Ocean *oceanTerrain = new Ocean();

	mainCamera = new Engine::Camera(
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::rotate(glm::mat4(1.0f), 0.5f * PI, glm::vec3(1, 0, 0))
	);
	mainCamera->SetProjection(g_window_width / g_window_height, 70.0, 0.1, 120);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	// glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
	// glfwSetCursorPosCallback(g_window, CursorPosCallback);
	glfwSetKeyCallback(g_window, KeyboardCallback);

	float prev_time = (float)glfwGetTime();
	float avg_elapsed_time = 1.0f / 60.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		avg_elapsed_time = avg_elapsed_time * (59.0 / 60.0) + elapsed_time * (1.0 / 60.0);
		prev_time = total_time;
		std::cout << "\33[2K\r";
		std::cout << "FPS: " << floor(1 / avg_elapsed_time);

		/* Clear buffers. */
		glClear(GL_COLOR_BUFFER_BIT | (mode_wireframe ? 0 : GL_DEPTH_BUFFER_BIT));

		/* Animate Objects. */
		oceanTerrain->Animate(total_time, mainCamera->GetTransform()->GetPosition());

		if (!mode_top_view)
		{
			if (mode_cam_left)
				mainCamera->GetTransform()->SetOrientation(
					glm::rotate(mainCamera->GetTransform()->GetOrientation(), CAMERA_ROTATE_SPEED * elapsed_time, glm::vec3(0, 1.0f, 0))
				);
			if (mode_cam_right)
				mainCamera->GetTransform()->SetOrientation(
					glm::rotate(mainCamera->GetTransform()->GetOrientation(), -CAMERA_ROTATE_SPEED * elapsed_time, glm::vec3(0, 1.0f, 0))
				);
		}

		

		/* Render pass. */
		oceanTerrain->Render(mainCamera);

		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}