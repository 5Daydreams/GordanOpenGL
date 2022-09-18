#include <iostream>
#include <filesystem>
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

#pragma region cubeMesh

GLfloat cubeVertices[] =
{ //     COORDINATES     //
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f
};

GLuint cubeIndices[] =
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

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.3f, 3.0f));

#pragma endregion

#pragma region Initializing Basic Renderers

	VAO quadVAO;
	quadVAO.Bind();

	VBO quadVBO(quadVertices, sizeof(quadVertices));
	EBO quadEBO(quadIndices, sizeof(quadIndices));

	quadVAO.LinkAttrib(quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	quadVAO.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//const VAO& VArrayObject, const EBO& IndexBufferObject, const Shader& shader
	Renderer quadRenderer(quadVAO, quadEBO);

#pragma endregion

#pragma region mainObject shader and VAO

	// Loading model
	std::string pathString = "Models/sphere.fbx";

	Model model = Model(pathString);

	// Generates Shader object using vert and frag shaders
	Shader shaderProgram("default.vert", "defaultPBR.frag");

#pragma endregion

#pragma region quad shader and VAO

	Shader quadShader("quadUV.vert", "quadUV.frag");

	VAO VAO2;
	VAO2.Bind();

	VBO VBO2(quadVertices, sizeof(quadVertices));
	EBO EBO2(quadIndices, 6);

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

#pragma endregion

#pragma region lightCube shader and VAO

	// The shader for the moving light cube
	Shader lightShader("light.vert", "light.frag");

	// Generates the Vertex Array for the light cube and binds it
	VAO lightVAO;
	lightVAO.Bind();

	// Generates the vertices and indices for the light
	VBO lightVBO(cubeVertices, sizeof(cubeVertices));
	EBO lightEBO(cubeIndices, sizeof(cubeIndices));

	// Links VBO attributes to VAO (in this case, its just vertex positions)
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// Unbind all buffers
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

#pragma endregion

#pragma region light object setup

	// Preparing Light Coloring
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 lightCubePos = glm::vec3(1.0f, 1.5f, 1.0f);
	glm::vec3 lightCubeScale = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::mat4 lightModelMatrix = glm::mat4(1.0f);
	lightModelMatrix = glm::scale(lightModelMatrix, lightCubeScale);
	lightModelMatrix = glm::translate(lightModelMatrix, lightCubePos);

	lightShader.Bind();
	lightShader.setMat4("model", lightModelMatrix);
	lightShader.setVec3("lightColor", lightColor);

#pragma endregion


#pragma region main object setup

	// Passing values to the MainObject shader
	glm::vec3 mainObjectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 mainObjectMatrix = glm::mat4(1.0f);
	mainObjectMatrix = glm::scale(mainObjectMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	mainObjectMatrix = glm::translate(mainObjectMatrix, mainObjectPos);

	shaderProgram.Bind();
	shaderProgram.setMat4("model", mainObjectMatrix);

	shaderProgram.setVec3("light.position", lightCubePos);
	shaderProgram.setVec3("light.color", lightColor);

	Texture albedo("Red_flat.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
	albedo.texUnit(shaderProgram, "albedo", 0);

	Texture normals("BaseNormals.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE);
	normals.texUnit(shaderProgram, "normalMap", 1);
	
	Texture metallic("FullBlack.png", GL_TEXTURE_2D, 2, GL_RGB, GL_UNSIGNED_BYTE);
	metallic.texUnit(shaderProgram, "metallic", 2);

	Texture roughness("FullWhite.png", GL_TEXTURE_2D, 3, GL_RGB, GL_UNSIGNED_BYTE);
	roughness.texUnit(shaderProgram, "roughness", 3);

	Texture ao("FullBlack.png", GL_TEXTURE_2D, 4, GL_RGB, GL_UNSIGNED_BYTE);
	ao.texUnit(shaderProgram, "ao", 4);

	float rot = 0.0f;

#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		lightShader.Bind();
		lightCubePos.x = 0.0f + 0.5f * (float)sin(glfwGetTime());
		lightCubePos.z = 0.0f + 0.5f * (float)cos(glfwGetTime());
		lightCubePos.y = 1.5f + 0.0f * (float)cos(glfwGetTime());
		lightModelMatrix = glm::translate(glm::mat4(1.0f), lightCubePos);
		lightModelMatrix = glm::scale(lightModelMatrix, lightCubeScale);
		lightShader.setMat4("model", lightModelMatrix);

		// camera events
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// setup for rendering the main object
		shaderProgram.Bind();
		albedo.Bind();
		normals.Bind();
		metallic.Bind();
		roughness.Bind();
		ao.Bind();

		shaderProgram.setVec3("camPos", camera.Position);
		mainObjectMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mainObjectMatrix = glm::rotate(mainObjectMatrix, rot, glm::vec3(0.0, 1.0, 1.0));
		mainObjectMatrix = glm::scale(mainObjectMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
		shaderProgram.setMat4("model", mainObjectMatrix);

		//rot += 0.01f;

		// Passing the camera position vector as a uniform to the object's shader file
		shaderProgram.setVec3("light.position", lightCubePos);
		//shaderProgram.setVec3("camPos", camera.Position.x, camera.Position.y, camera.Position.z);

		// Passing the camera model * projection matrix as a uniform to the object's shader file
		camera.MatrixUniform(shaderProgram, "camMatrix");

		// Regularly drawing the object 
		model.Draw(shaderProgram);

		//cubeVAO.Bind();
		//mainObjectMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		//shaderProgram.setMat4("model", mainObjectMatrix);
		//GLCall(glDrawElements(GL_TRIANGLES, cubeEBO.count, GL_UNSIGNED_INT, 0));

		// setup for rendering the light cube
		lightShader.Bind();
		camera.MatrixUniform(lightShader, "camMatrix");
		lightVAO.Bind();

		const int indexCountLight = sizeof(cubeIndices) / sizeof(int);
		GLCall(glDrawElements(GL_TRIANGLES, indexCountLight, GL_UNSIGNED_INT, 0));

		// Swap render buffers
		glfwSwapBuffers(window);
		// handle window events, I think also inputs?
		glfwPollEvents();

		ExitOnEsc(window);
	}

	// Deleting the abstracted GPU objects
	shaderProgram.Delete();
	albedo.Delete();
	normals.Delete();
	metallic.Delete();
	roughness.Delete();
	ao.Delete();

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
