#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

double global_time;

int main()
{
	glfwInit();

	// window hints, information necessary to create wwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	GLFWmonitor* fullscreen = NULL;
	int width = 800;
	int height = 600;
	GLFWwindow* window = glfwCreateWindow(width, height, "I am a window, my name is Doorothy", fullscreen, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window. \n" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Window exists, now make it the current context
	glfwMakeContextCurrent(window);

	// glad loads the openGL functions
	gladLoadGL();

	// viewport size, as in, where we want to render at
	glViewport(0,0, width, height);


	// camera's clear flags as a solid color
	glClearColor(0.07f,0.13f,0.17f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	while(!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		// if this is not done, window will be unresponsive
		glfwPollEvents();
	}

	// cleaning up window reference and glfw context
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}