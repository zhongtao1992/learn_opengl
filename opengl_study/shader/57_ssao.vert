#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out VS_OUT{
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
} vs_out;

void main(){
	vs_out.v_texCoords = aTexCoords;

    gl_Position = vec4(aPos, 1.0);
}