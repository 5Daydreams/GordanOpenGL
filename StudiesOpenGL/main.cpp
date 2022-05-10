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
{ //     COORDS       /     UV_COORDS    //
	-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
	+1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
	+1.0f, +1.0f, 0.0f,     1.0f, 1.0f,
	-1.0f, +1.0f, 0.0f,     0.0f, 1.0f
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
{ //     COORDINATES    /    TexCoord    /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.0f, 2.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     2.0f, 2.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     2.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     2.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     2.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     2.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     2.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     1.0f, 2.0f,      0.0f, 0.5f,  0.8f  // Facing side
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

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

void ExitOnEsc(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
#pragma region window setup

	glfwInit();

	// window hints, information necessary to create wwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	// viewport size, as in, where we want to render at
	GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

#pragma endregion

#pragma region mainObject shader and VAO

	// Generates Shader object using defualt.vert and default.frag shaders
	Shader shaderProgram("default.vert", "multiLight.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(verticesPyramidLighting, sizeof(verticesPyramidLighting));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indicesPyramidLighting, sizeof(indicesPyramidLighting));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	// (!) As of right now in the code, its safe to assume that the VBO's are highly packed and not particularly modularized, 
	// or at least there should be a better way to structure their data 

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

#pragma endregion

#pragma region quad shader and VAO

	Shader quadShader("quadUV.vert", "quadUV.frag");

	VAO VAO2;
	VAO2.Bind();

	VBO VBO2(verticesQuad, sizeof(verticesQuad));
	EBO EBO2(indicesQuad, sizeof(indicesQuad));

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

#pragma endregion

#pragma region lightCube shader and VAO

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

#pragma endregion

	// Preparing Light Coloring
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 lightColor = lightDiffuse;

	glm::vec3 lightPos = glm::vec3(1.0f, 1.5f, 1.0f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::translate(lightModelMatrix, lightPos);

	lightShader.Activate();
	lightShader.setMat4("model", lightModelMatrix);
	lightShader.setVec3("lightColor", lightColor);
	
	// Passing values to the MainObject shader
	glm::vec3 mainObjectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 mainObjectMatrix = glm::mat4(1.0f);
	mainObjectMatrix = glm::scale(mainObjectMatrix, glm::vec3(1.0f,1.0f,1.0f));
	mainObjectMatrix = glm::translate(mainObjectMatrix, mainObjectPos);

	shaderProgram.Activate();
	shaderProgram.setMat4("model", mainObjectMatrix);
	shaderProgram.setFloat("material.shininess", 32.0f);

	if (false) // set to true for directional lighting setup
	{
		glm::vec3 lightDir = glm::vec3(0.3f, -1.0f, 0.3f);
		shaderProgram.setVec3("light.direction", lightDir);
	}

	if (false) // set to true for point lighting setup
	{
		shaderProgram.setVec3("light.position", lightPos);
		shaderProgram.setFloat("light.linearFalloff", 0.01f);
		shaderProgram.setFloat("light.quadraticFalloff", 0.032f);
	}

	if (true) // set to true for spotlight setup
	{
		shaderProgram.setVec3("spotlight.position", lightPos);
		glm::vec3 spotlightDir = glm::vec3(- 1.0f, -1.0f, -1.0f);
		shaderProgram.setVec3("spotlight.spotDirection", spotlightDir);

		GLfloat innerCutoffValue = glm::cos(glm::radians(8.0f));
		GLfloat outerCutoffValue = glm::cos(glm::radians(12.0f));

		shaderProgram.setFloat("spotlight.innerCutoff", innerCutoffValue);
		shaderProgram.setFloat("spotlight.outerCutoff", outerCutoffValue);
		
		shaderProgram.setFloat("spotlight.linearFalloff", 0.03f);
		shaderProgram.setFloat("spotlight.quadraticFalloff", 0.05f);
	}

	shaderProgram.setVec3("spotlight.ambient", lightAmbient);
	shaderProgram.setVec3("spotlight.diffuse", lightDiffuse);
	shaderProgram.setVec3("spotlight.specular", lightSpecular);

	Texture texture("PebblesTile.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "material.albedo", 0);
	Texture specular("PebblesTileSpecular.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	specular.texUnit(shaderProgram, "material.specular", 1);

	quadShader.Activate();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(quadShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(mainObjectMatrix)));

	GLCall(glEnable(GL_DEPTH_TEST));


	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		lightShader.Activate();
		lightPos.x = (float)sin(glfwGetTime());
		lightPos.z = (float)cos(glfwGetTime());
		lightModelMatrix = glm::translate(glm::mat4(1.0f), lightPos);
		GLCall(glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModelMatrix)));

		// camera events
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// setup for rendering the main opject
		shaderProgram.Activate();

		// Passing the camera position vector as a uniform to the object's shader file
		GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "spotlight.position"), lightPos.x, lightPos.y, lightPos.z));
		glm::vec3 spotlightDir = mainObjectPos - lightPos;
		shaderProgram.setVec3("spotlight.spotDirection", spotlightDir);
		GLCall(glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z));
		// Passing the camera model * projection matrix as a uniform to the object's shader file
		camera.MatrixUniform(shaderProgram, "camMatrix");

		texture.Bind();
		specular.Bind();
		VAO1.Bind();

		const int indexCount = sizeof(indicesPyramidLighting) / sizeof(int);
		// GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));

		// setup for rendering the light cube
		lightShader.Activate();
		camera.MatrixUniform(lightShader, "camMatrix");
		lightVAO.Bind();

		const int indexCountLight = sizeof(lightIndices) / sizeof(int);
		// GLCall(glDrawElements(GL_TRIANGLES, indexCountLight, GL_UNSIGNED_INT, 0));

		quadShader.Activate();
		camera.MatrixUniform(quadShader, "camMatrix");
		VAO2.Bind();
		const int indexCountQuad = sizeof(indicesQuad) / sizeof(int);
		GLCall(glDrawElements(GL_TRIANGLES, indexCountQuad , GL_UNSIGNED_INT, 0));

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

	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	quadShader.Delete();

	// cleaning up window reference and then glfw context (!! IN THAT ORDER !!)
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}