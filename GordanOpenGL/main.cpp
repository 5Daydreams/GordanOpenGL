#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

GLfloat verticesTri[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
	+0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
	+0.0f, +0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
	-0.5f / 2, +0.5f * float(sqrt(3)) / 6, 0.0f,
	+0.5f / 2, +0.5f * float(sqrt(3)) / 6, 0.0f,
	+0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
};

GLfloat verticesQuad[] =
{
	-0.5f,-0.5f, 0.0f,
	+0.5f,-0.5f, 0.0f,
	+0.5f,+0.5f, 0.0f,
	-0.5f,+0.5f, 0.0f
};

GLuint indicesTri[]
{
	0, 3, 5,
	3, 2, 4,
	5, 4, 1
};

GLuint indicesQuad[]
{
	0, 1, 2,
	0, 2, 3
};


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
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Window exists, now make it the current context
	glfwMakeContextCurrent(window);

	// glad loads the openGL functions
	gladLoadGL();

	// viewport size, as in, where we want to render at
	glViewport(0, 0, width, height);

	// Generates Shader object using defualt.vert and default.frag shaders
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(verticesTri, sizeof(verticesTri));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indicesTri, sizeof(indicesTri));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		VAO1.Bind();

		//const int startingIndex = 0;
		//const int vertexCount = 3;
		//glDrawArrays(GL_TRIANGLES, startingIndex, vertexCount);

		const int startingIndex = 0;
		const int indexCount = 9;
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		// if this is not done, window will be unresponsive
		glfwPollEvents();
	}

	// Deleting abstracted GPU objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// cleaning up window reference and then glfw context (YES, IN THAT ORDER!!)
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}