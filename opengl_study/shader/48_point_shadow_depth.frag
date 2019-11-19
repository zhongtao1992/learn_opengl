#version 330 core
in vec4 FragPos;

uniform vec3 u_lightPos;
uniform float u_far_plane;

void main(){
	float lightDis = length(FragPos.xyz - u_lightPos);

	lightDis = lightDis / u_far_plane;

	gl_FragDepth = lightDis;
}