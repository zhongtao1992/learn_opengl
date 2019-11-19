#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedoSpec;

in VS_OUT{
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
} fs_in;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
	float shininess;	
};
uniform Material u_material;

void main(){
    gPosition = fs_in.v_fragPos;
    gNormal = normalize(fs_in.v_normal);
   	gAlbedoSpec.rgb = vec3(0.95);
}