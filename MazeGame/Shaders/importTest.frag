#pragma glslify: levels = require('./levels')

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

void main() {
    float value = levels(0.7, 0.0, 0.6, 0.0).r;

    gl_FragColor = vec4(value,0,0,1);
}