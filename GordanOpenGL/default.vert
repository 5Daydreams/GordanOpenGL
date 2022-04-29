#version 330 core

// Vertex Pos
layout (location = 0) in vec3 aPos;
// Vertex Colors
layout (location = 1) in vec3 aColor;
// TextureCoords
layout (location = 2) in vec2 aTex;
// Normals (not necessarily normalized)
layout (location = 3) in vec3 aNormal;

// out means this will be passed to the frag shaders
out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 crntPos;

// Imports the camera view * projection matrix from the main function
uniform mat4 camMatrix;
// Imports the object's model matrix from the main function
uniform mat4 model;

void main()
{
	// adjusting position to world space 
	crntPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(crntPos, 1.0);
	
	// setting values into "out" variables
	color = aColor;
	texCoord = aTex;
	Normal = aNormal;
}
