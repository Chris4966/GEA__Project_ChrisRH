#version 410 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 mMatrix;
uniform mat4 vpMatrix;

void main()
{
    gl_Position =  vpMatrix * mMatrix * vec4(vertexPosition,1);
}
