#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 v_TexCoords;
out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform bool u_reverse_normal;

void main(){
	vs_out.FragPos = vec3(u_model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(u_model))) * aNormal;
	vs_out.TexCoords = aTexCoords;
	if (u_reverse_normal){
		vs_out.Normal = transpose(inverse(mat3(u_model))) * aNormal * -1.0;
	}
	else{
		vs_out.Normal = transpose(inverse(mat3(u_model))) * aNormal;
	}

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}