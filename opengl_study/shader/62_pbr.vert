#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

out VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
	vec3 v_worldPos;
} vs_out;

void main(){
	vs_out.v_worldPos = vec3(u_model * vec4(aPos, 1.0f));
	vs_out.v_texCoords = aTexCoords;
    vs_out.v_normal = mat3(u_model) * aNormal;

    gl_Position = u_projection * u_view * vec4(vs_out.v_worldPos, 1.0f);
}