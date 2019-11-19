#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 v_TexCoords;
out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

void main(){
	vs_out.FragPos = vec3(u_model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(u_model))) * aNormal;
	vs_out.TexCoords = aTexCoords;
	vs_out.FragPosLightSpace = u_lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}