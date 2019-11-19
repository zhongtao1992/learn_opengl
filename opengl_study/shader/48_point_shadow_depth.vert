#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_mode;

void main(){
    gl_Position = u_mode * vec4(aPos, 1.0);
}