#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

// out means this will be passed to the frag shaders
out vec2 texCoord;

// Imports the "cameraView * cameraProjection" matrix from the main function
uniform mat4 camMatrix;

// Imports the object's model matrix from the main function
uniform mat4 model;

void main()
{
	// adjusting position to world space 
	vec3 crntPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(crntPos, 1.0);
	
	// setting values into "out" variables
	texCoord = aTex;
}
