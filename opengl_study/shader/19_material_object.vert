#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 oNormal;
out vec3 oFragPos;
out vec2 oTexCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
    oNormal = aNormal;
    oFragPos = vec3(u_model * vec4(aPos, 1.0f));
    oTexCoords = aTexCoords;
}