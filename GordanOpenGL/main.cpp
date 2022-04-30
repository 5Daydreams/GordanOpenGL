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

GLfloat verticesPyramidLighting[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 2.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 2.0f, 2.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 2.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 2.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 1.0f, 2.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 2.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 1.0f, 2.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 2.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 1.0f, 2.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 2.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 1.0f, 2.0f,      0.0f, 0.5f,  0.8f  // Facing side
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

GLuint indicesPyramidLighting[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

#pragma endregion

#pragma region lightMesh

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

#pragma endregion

int width = 800;
int height = 600;

void ExitOnEsc(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


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
	GLCall(glViewport(0, 0, width, height));

	// Generates Shader object using defualt.vert and default.frag shaders
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(verticesPyramidLighting, sizeof(verticesPyramidLighting));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indicesPyramidLighting, sizeof(indicesPyramidLighting));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	// (!) As of right now in the code, its safe to assume that the VBO's are highly packed and not particularly modularized, 
	// or at least there should be a better way to structure their data 

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// The shader for the light cube
	Shader lightShader("light.vert", "light.frag");

	// Generates the Vertex Array for the light cube and binds it
	VAO lightVAO;
	lightVAO.Bind();

	// Generates the vertices and indices for the light
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	// Links VBO attributes to VAO (in this case, its just vertex positions)
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// Unbind all buffers
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 1.5f, 0.5f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::translate(lightModelMatrix, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModelMatrix = glm::mat4(1.0f);
	pyramidModelMatrix = glm::translate(pyramidModelMatrix, pyramidPos);


	lightShader.Activate();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModelMatrix)));
	GLCall(glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w));
	shaderProgram.Activate();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModelMatrix)));
	GLCall(glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w));
	GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z));


	Texture texture("PebblesTile.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "tex0", 0);
	Texture specular("PebblesTileSpecular.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	specular.texUnit(shaderProgram, "tex1", 1);

	GLCall(glEnable(GL_DEPTH_TEST));

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		lightShader.Activate();
		lightPos.x = (float) sin(glfwGetTime());
		lightPos.z = (float) cos(glfwGetTime());
		lightModelMatrix = glm::translate(glm::mat4(1.0f), lightPos);
		GLCall(glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModelMatrix)));

		// camera events
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// setup for rendering the main opject
		shaderProgram.Activate();

		// Passing the camera position vector as a uniform to the object's shader file
		GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z));
		GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z));
		// Passing the camera model * projection matrix as a uniform to the object's shader file
		camera.MatrixUniform(shaderProgram, "camMatrix");

		texture.Bind();
		specular.Bind();
		VAO1.Bind();

		const int indexCount = sizeof(indicesPyramidLighting) / sizeof(int);
		GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));

		// setup for rendering the light cube
		lightShader.Activate();
		camera.MatrixUniform(lightShader, "camMatrix");
		lightVAO.Bind();

		const int indexCountLight = sizeof(lightIndices) / sizeof(int);
		GLCall(glDrawElements(GL_TRIANGLES, indexCountLight, GL_UNSIGNED_INT, 0));

		// Swap render buffers
		glfwSwapBuffers(window);
		// handle window events
		glfwPollEvents();

		ExitOnEsc(window);
	}

	// Deleting the abstracted GPU objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	// cleaning up window reference and then glfw context (IN THAT ORDER!!)
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}