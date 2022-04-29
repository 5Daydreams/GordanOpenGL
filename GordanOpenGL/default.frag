#version 330 core

// out in frag shaders means its the pixel color output
out vec4 FragColor;

// the "in" keyword means these values are taken from the vertex shader within this particular shader program
in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;

// uniforms represent the _same value_ for all fragments within a shader instace, 
// hard-push to use "uniform" instead of "varying" variables, as those can take up significantly more time to process
uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularGradient = max(dot(viewDirection, reflectionDirection), 0.0f);
	float specAmount = pow(specularGradient, 8);
	
	float specularLight = 0.50f;
	float specular = specAmount * specularLight;

	// outputs final color
	vec4 texColor = texture(tex0, texCoord);
	FragColor = texColor * lightColor * (diffuse + ambient + specular);
}