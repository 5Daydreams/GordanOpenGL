#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

#pragma region vertexMeshes

GLfloat verticesTri[] =
{ //               COORDS                        /       COLORS          //
	-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,   0.80f, 0.30f, 0.02f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,   0.80f, 0.30f, 0.02f, // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.00f, 0.60f, 0.32f, // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,   0.00f, 0.45f, 0.77f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,   0.00f, 0.45f, 0.77f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,   0.20f, 0.30f, 0.80f  // Inner down
};

GLfloat verticesQuad[] =
{ //    COORDS                /       COLORS       /    UV_COORDS    //
	-0.5f,-0.5f, 0.0f,         0.80f, 0.30f, 0.02f,	    0.0f, 0.0f,
	+0.5f,-0.5f, 0.0f,         0.80f, 0.30f, 0.02f,	    1.0f, 0.0f,
	+0.5f,+0.5f, 0.0f,         1.00f, 0.60f, 0.32f,	    1.0f, 1.0f,
	-0.5f,+0.5f, 0.0f,         0.00f, 0.45f, 0.77f,     0.0f, 1.0f
};

GLfloat verticesPyramid[] =
{ //    COORDS            /       COLORS       /      UV_COORDS    //
	-0.5f, +0.0f, +0.5f,     0.80f, 0.30f, 0.02f,	  0.0f, 1.0f,
	-0.5f, +0.0f, -0.5f,     0.80f, 0.30f, 0.02f,	  0.0f, 0.0f,
	+0.5f, +0.0f, -0.5f,     0.80f, 0.30f, 0.02f,	  1.0f, 0.0f,
	+0.5f, +0.0f, +0.5f,     1.00f, 0.60f, 0.32f,	  0.0f, 0.0f,
	+0.0f, +0.8f, +0.0f,     0.00f, 0.45f, 0.77f,     1.0f, 1.0f
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

GLuint indicesPyramid[]
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

#pragma endregion

int width = 800;
int height = 600;

int main()
{
	glfwInit();

	// window hints, information necessary to create wwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window
	GLFWmonitor* fullscreen = NULL;

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
	VBO VBO1(verticesPyramid, sizeof(verticesPyramid));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indicesPyramid, sizeof(indicesPyramid));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// (!) As of right now in the code, its safe to assume that the VBO's are highly packed and not particularly modularized, 
	// or at least there should be a better way to structure their data 

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture texture("Ghost_Alvin_512.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f,0.0f,2.0f));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		
		texture.Bind();
		VAO1.Bind();

		const int startingIndex = 0;
		const int indexCount = 3 * 6;
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		// if this is not done, window will be unresponsive
		glfwPollEvents();
	}

	// Deleting abstracted GPU objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();

	// cleaning up window reference and then glfw context (YES, IN THAT ORDER!!)
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}