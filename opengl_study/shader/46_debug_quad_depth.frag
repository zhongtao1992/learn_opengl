#version 330 core
out vec4 FragColor;


in VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
} fs_in;
uniform sampler2D u_depthTex;
uniform float u_near_plane;
uniform float u_far_plane;

float linearizeDepth(float depth){
	float z = depth * 2.0 - 1.0;
	return (2.0 * u_near_plane * u_far_plane) / (u_far_plane + u_near_plane - z * (u_far_plane - u_near_plane));
}

void main(){
	float depthValue = texture(u_depthTex, fs_in.v_texCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0f);
}