#version 330 core

// Vertex Pos
layout (location = 0) in vec3 aPos;
// Vertex Colors
layout (location = 1) in vec3 aColor;
// TextureCoords
layout (location = 2) in vec2 aTex;

// out means this will be passed to the frag shaders
out vec3 color;

out vec2 texCoord;

uniform mat4 camMatrix;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);
	
	// Assigns the Vertex Colors to the "color" out-vec3 (needs to be out to be passed between from vertex to fragment shaders)
	color = aColor;
	texCoord = aTex;
}
