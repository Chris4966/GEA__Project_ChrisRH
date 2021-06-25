#version 410 core
out vec4 fragColor;
//out vec3 mNormal;

//in vec3 Normal;

uniform vec3 objectColor;

void main() {

    fragColor = vec4(objectColor, 1.f);
}
