#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflection1;
	float shininess;	
};

in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoords;

uniform vec3 u_cameraPos;
uniform samplerCube u_skybox;
uniform Material u_material;

void main(){
	vec3 I = normalize(v_Position - u_cameraPos);
	vec3 R = reflect(I, normalize(v_Normal));
	vec3 reflCol = texture(u_skybox, R).rgb;
	vec3 reflTex = texture(u_material.texture_reflection1, v_TexCoords).rgb;
	vec3 sumRef = mix(reflCol, reflTex, 0.5);

	vec3 diffuseCol = texture(u_material.texture_diffuse1, v_TexCoords).rgb;
	vec3 specularColr = texture(u_material.texture_specular1, v_TexCoords).rgb;

	vec3 modelCol = 0.34 * diffuseCol + 0.75 * sumRef * specularColr;

	FragColor = vec4(modelCol, 1.0);
}