#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Model.h"
#include "ModelBuilder.h"
#include "Renderer.h"

#pragma region quadMesh

GLfloat quadVertices[] =
{ //     COORDS       /     UV_COORDS    //
	-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
	+1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
	+1.0f, +1.0f, 0.0f,     1.0f, 1.0f,
	-1.0f, +1.0f, 0.0f,     0.0f, 1.0f
};

GLuint quadIndices[]
{
	0, 1, 2,
	0, 2, 3
};

#pragma endregion

const unsigned int SCREEN_WIDTH = 1024;
const unsigned int SCREEN_HEIGHT = 800;

void ExitOnEsc(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
#pragma region window and camera setup

	glfwInit();

	// window hints, information necessary to create wwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window - notice the distinction between Window and Monitor - 
	GLFWmonitor* fullscreen = NULL;
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "I am a window, my name is Doorothy", fullscreen, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Window exists, now make it the current context
	glfwMakeContextCurrent(window);

	// glad loads the openGL functions
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	// Fixed that thing that Unreal does, where it flips textures upside down
	stbi_set_flip_vertically_on_load(true);

	// viewport size, as in, where we want to render at
	GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	// Enables Depth test, prioritizing closer objects (menmonic: LESS distant)
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 1.8f));

#pragma endregion

#pragma region Initializing Quad

	VAO quadVAO;
	quadVAO.Bind();

	VBO quadVBO(quadVertices, sizeof(quadVertices));
	EBO quadEBO(quadIndices, sizeof(quadIndices));

	quadVAO.LinkAttrib(quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	quadVAO.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//const VAO& VArrayObject, const EBO& IndexBufferObject, const Shader& shader
	Renderer quadRenderer(quadVAO, quadEBO);

	// Generates Shader object using vert and frag shaders
	Shader quadShader("quadUV.vert", "quadUV.frag");

	// Are these really necessary?
	quadVAO.Unbind();
	quadVBO.Unbind();
	quadEBO.Unbind();

#pragma endregion

	float rot = 0.0f;
	float time = 0.0f;

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// camera events
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// setup for rendering the main object
		quadShader.Bind();

		//rot += 0.01f;
		time += 0.01f;

		// Passing the camera model * projection matrix as a uniform to the object's shader file
		camera.MatrixUniform(quadShader, "camMatrix");

		quadRenderer.Draw(quadShader);

		// Swap render buffers
		glfwSwapBuffers(window);
		// handle window events, I think also inputs?
		glfwPollEvents();

		ExitOnEsc(window);
	}

	// Deleting the abstracted GPU objects
	quadShader.Delete();

	quadVAO.Delete();
	quadVBO.Delete();
	quadEBO.Delete();
	quadShader.Delete();

	// cleaning up window reference and then glfw context (!! IN THAT ORDER !!)
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
