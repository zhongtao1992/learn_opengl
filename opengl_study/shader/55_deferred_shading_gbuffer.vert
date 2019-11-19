#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
} vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
	vs_out.v_texCoords = aTexCoords;
	vs_out.v_fragPos = vec3(u_model * vec4(aPos, 1.0));

	mat3 normalMatrix = transpose(inverse(mat3(u_model)));
	vs_out.v_normal = normalMatrix * aNormal;

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}