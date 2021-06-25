#version 410 core
layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec3 vertexNormal;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {

   gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition, 1.0);
}
