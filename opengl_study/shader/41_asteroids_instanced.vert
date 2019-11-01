#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aModel;

uniform mat4 u_view;
uniform mat4 u_projection;
out vec2 v_texCoords;

void main(){
    gl_Position = u_projection * u_view * aModel * vec4(aPos, 1.0);
    v_texCoords = aTexCoords;
}