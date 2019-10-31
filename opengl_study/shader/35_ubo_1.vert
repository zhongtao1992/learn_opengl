#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
// uniform buffer
layout (std140) uniform Matrices{
	mat4 u_projection;
	mat4 u_view;
};

out vec2 inTexCoords;

uniform mat4 u_model;

void main(){
	inTexCoords = aTexCoords;
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}