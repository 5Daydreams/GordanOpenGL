#version 330 core

out vec4 FragColor;

// Inputs color from the Vertex Shader (note the vec3 as opposed to vec 4 format)
in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
}