#version 330 core

out vec4 FragColor;

// Inputs color from the Vertex Shader (note the vec3 as opposed to vec 4 format)
in vec3 color;

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord) * vec4(color,1.0f);
}