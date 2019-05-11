// STL
#include <iostream>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>
#include <ModelLoader.hpp>

#include <DiffuseMaterial.hpp>
#include <PickingMaterial.hpp>
#include <Geometry.hpp>
#include <ModelLoader.hpp>

#include <picking.hpp>
#include <PickableObject.hpp>

#include <PhongMaterial.hpp>
#include <ToonMaterial.hpp>
#include <MyMaterial.hpp>
#include <Animation.hpp>

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

Engine::Camera* main_camera;

Engine::Mesh* ground_mesh;
Engine::Mesh* bunny_mesh;

DiffuseMaterial* material;
Engine::RenderObject* ground_object;
Engine::RenderObject* bunny_object;

std::vector<Light> lights;

Animation* bunny_animation;
Animation* camera_animation;
Animation* light_animation;

static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(a_window, &xpos, &ypos);
		xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
		ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
		int target = pick((int)xpos, (int)ypos, g_framebuffer_width, g_framebuffer_height);
	}
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
		case GLFW_KEY_H:
			std::cout << "CS380 Homework 3" << std::endl;
			std::cout << "keymaps:" << std::endl;
			std::cout << "h\t\t Help command" << std::endl;
			break;
		default:
			break;
		}
	}
}

static void FramebufferResizeCallback(GLFWwindow* a_window, int width, int height)
{
	glViewport(0, 0, width, height);
	reallocatePickingTexture(width, height);

	int window_width, window_height;
	glfwGetWindowSize(g_window, &window_width, &window_height);
	g_window_width = (float)window_width;
	g_window_height = (float)window_height;
	g_framebuffer_width = width;
	g_framebuffer_height = height;
	
	Engine::ProjectionData proj = main_camera->GetProjection();
	proj.aspectRatio = (float)width / (float)height;
	main_camera->SetProjection(proj.aspectRatio, proj.fov, proj.zNear, proj.zFar);
}

static void InitScene()
{
	main_camera = new Engine::Camera();
	main_camera->GetTransform()->SetPosition(glm::vec3(0.0f, 1.0f, 4.0f));

	Engine::ModelLoader *loader = new Engine::ModelLoader("Resources\\Models\\scene.obj");

	ground_mesh = new Engine::Mesh();
	bunny_mesh = new Engine::Mesh();
	loader->GenerateMesh(ground_mesh, 0);
	loader->GenerateMesh(bunny_mesh, 1);

	material = new DiffuseMaterial();
	material->CreateMaterial();

	ground_object = new Engine::RenderObject(ground_mesh, material);
	bunny_object = new Engine::RenderObject(bunny_mesh, material);
	
	Light sun = Light();
	sun.transform.SetPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	sun.diffuse_illuminance = glm::vec3(1.0f, 0.5f, 0.0f);
	lights.push_back(sun);
}

static void InitAnimation()
{
	/*
		This is an example animated scene using Animation.
		Animation is an keyframe based animating tool that do certain movement repeatedly.
		It is completely optional to utilize Animation, but it would be really helpful for HW3 :)
		For more details, see Animation.hpp


		1. Jumping bunny
		This animation is about transformation of bunny, repeated every 5 seconds,
			and will affect position, orientation, and scale of the bunny.
		There are total 3 keyframes: at 0s, at 3s and at 4s
		0-----1-----2-----3-----4-----5(=0)
		x-----------------x-----x-----|
		^                 ^     ^
		|                 |     |
		+-- In this moment, bunny is located at (-1, 0, 0)
							bunny is rotated -80 degrees in y axis
							bunny is scaled to (1, 1, 1) - default scale value of Engine::Transform
						  |     |
						  +-- In this moment, bunny is located at (1, 0, 0)
								|			  bunny is rotated 80 degress in y axis
								|			  bunny is scaled to (1, 0.5, 1) - ready to jump
								|
								+-- In this moment, bunny is located at (0, 4, 0)
													bunny is rotated 180 degrees in x axis - upside down!
													bunny is scaled to (1, 2, 1) - stretching up
		
		Between keyframes, Animation smoothly fill in the transformation state of the bunny,
			so you can see the smooth result.
	*/
	bunny_animation = new Animation(bunny_object->GetTransform(), 5, AnimationType::All);

	Engine::Transform bunny_0;
	bunny_0.SetPosition(glm::vec3(-1.0, 0.0f, 0.0f));
	bunny_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-80.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	bunny_animation->AddKeyframe(0.0f, bunny_0);
	
	Engine::Transform bunny_1;
	bunny_1.SetPosition(glm::vec3(1.0, 0.0f, 0.0f));
	bunny_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	bunny_1.SetScale(glm::vec3(1.0f, 0.5f, 1.0f));
	bunny_animation->AddKeyframe(3.0f, bunny_1);

	Engine::Transform bunny_2;
	bunny_2.SetPosition(glm::vec3(0.0, 4.0f, 0.0f));
	bunny_2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	bunny_2.SetScale(glm::vec3(1.0f, 2.0f, 1.0f));
	bunny_animation->AddKeyframe(4.0f, bunny_2);

	/*
		2. Moving camera
		Unlike previous homeworks, Engine::Camera shares the same position and orientation structure (Engien::Transform) as Engine::RenderObject.
		So, you can animate a Camera as easily as a RenderObject.

		This animation is about main camera, repeats every 3 seconds, and only affects position and orientation.
	*/
	camera_animation = new Animation(main_camera->GetTransform(), 3, AnimationType::Position | AnimationType::Orientation);

	Engine::Transform camera_0;
	camera_0.SetPosition(glm::vec3(0.0f, 1.0f, 4.0f));
	camera_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	camera_animation->AddKeyframe(0.0f, camera_0);

	Engine::Transform camera_1;
	camera_1.SetPosition(glm::vec3(0.0f, 2.0f, 4.0f));
	camera_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	camera_animation->AddKeyframe(1.5f, camera_1);

	/*
		There is another benefit for having shared transformation structure: now camera can be in a hierarchical structure!
		To see the effect, try run after uncommenting below line.
	*/
	//main_camera->GetTransform()->AddParent(bunny_object->GetTransform());

	/*
		3. Moving lights
		Light can have position and orientation, so why not use the same Engine::Transform?
		In this way, you can benefit from Animation by animating lights with keyframes.
		Unfortunately, Animation only support smooth changes of Engine::Transform.
		If you want to smoothly change the color of light, tough luck.
		Or you can modify Animation to support such behaviors, but it is out of scope of this homework.

		Note that your implementation of Light does not require to use Engine::Transform.
		However, if so, it will enable more freedom to your scene composition!
		(e.g. using Animation for smooth animation, put lights in a hierarchy, ...)
	*/
	light_animation = new Animation(&lights[0].transform, 1, AnimationType::Position);

	Engine::Transform light_0;
	light_0.SetPosition(glm::vec3(10.0, 10.0f, 0.0f));
	light_animation->AddKeyframe(0.0f, light_0);

	Engine::Transform light_1;
	light_1.SetPosition(glm::vec3(-10.0, -10.0f, 0.0f));
	light_animation->AddKeyframe(0.5f, light_1);

	/*
		Now the animations are ready to animate!
		All you need to do is simply invoke Animation::Animate() in a main loop!
	*/
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

	/* Create a windowed mode window and its OpenGL context */
	g_window = glfwCreateWindow(g_window_width, g_window_height, "Homework 3 - Let there be light and Blinn-Phong reflection model", NULL, NULL);
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


	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
	glfwSetCursorPosCallback(g_window, CursorPosCallback);
	glfwSetKeyCallback(g_window, KeyboardCallback);
	glfwSetFramebufferSizeCallback(g_window, FramebufferResizeCallback);

	// Initialize framebuffer object and picking textures
	pickingInitialize(g_framebuffer_width, g_framebuffer_width);

	InitScene();
	InitAnimation();

	float prev_time = (float)glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		prev_time = total_time;

		// First Pass: Object Selection
		// this is for picking the object using mouse interaction
		// binding framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
		// Background: RGB = 0x000000 => objectID: 0
		glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* RenderPicking here */

		// Second Pass: Object Rendering
		// Drawing object again
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor((GLclampf)(128. / 255.), (GLclampf)(200. / 255.), (GLclampf)(255. / 255.), (GLclampf)0.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render here */
		bunny_animation->Animate(elapsed_time);		// Please don't forget to call Animation.Animate in each frame!
		camera_animation->Animate(elapsed_time);
		light_animation->Animate(elapsed_time);

		material->UpdateLight(lights);
		material->UpdateDiffuseReflectance(glm::vec3(0.1f, 0.9f, 0.1f));
		ground_object->Render(main_camera);

		material->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.9f, 0.9f));
		bunny_object->Render(main_camera);


		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete resources
	delete main_camera;

	glfwTerminate();
	return 0;

}