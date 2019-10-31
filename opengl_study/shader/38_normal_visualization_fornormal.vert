#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 normal;
} vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
    
    mat3 normalMatrix = mat3(transpose(inverse(u_view * u_model)));
    vs_out.normal = normalize(vec3(u_projection * vec4(normalMatrix * aNormal, 0.0)));
}