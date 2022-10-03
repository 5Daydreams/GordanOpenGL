#version 330 core

// Values to be filled by the VAO
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

// out means these must be cached in the vert.main() call to be available within frag.main()
out vec3 FragPos;
out vec3 Normal;
out vec2 texCoord;

// Uniforms are values to be set during the application's runtime (either on startup or loop)
uniform mat4 camMatrix;
uniform mat4 model;
uniform float time;

void main()
{
	vec3 originalPos = vec3(model * vec4(aPos, 1.0f));

	// setting values into "out" variables
    Normal = mat3(transpose(inverse(model))) * aNormal;  
	FragPos = originalPos + Normal * (sin(originalPos.y*10 + time*2) * 0.5f + 0.5f) * 0.01f;
	texCoord = aTex;

	// this is the vec4 position of the current vertex in the CAMERA's world space
	// Notice the subtle distinction - the FragPos variable is going to be used for calculating lighting, 
	// therefore it has to be in regular world space, while this one is where "from the camera's perspective" the object will be
	gl_Position = camMatrix * vec4(FragPos, 1.0);
}
