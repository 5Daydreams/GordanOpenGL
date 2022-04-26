#version 330 core

// Vertex Pos
layout (location = 0) in vec3 aPos;
// Vertex Colors
layout (location = 1) in vec3 aColor;

// out means this will be passed to the frag shaders
out vec3 color;

// uniform is constant for all elements in this shader stage, and is shared between vert/frag
uniform float scale;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	
	// Assigns the Vertex Colors to the "color" out-vec3 (needs to be out to be passed between from vertex to fragment shaders)
	color = aColor;
}
