#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 v_Normal;
out vec3 v_Position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
	v_Normal = mat3(transpose(inverse(u_model))) * aNormal;
	v_Position = vec3(u_model * vec4(aPos, 1.0));

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}