#version 410 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;

out vec4 col;
uniform mat4 mMatrix;

void main() {
   col = colAttr;
   gl_Position = mMatrix * posAttr;
}
