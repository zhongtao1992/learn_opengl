#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;

    sampler2D texture_diffuse1;
	float shininess;	
};

in VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
} fs_in;

uniform Material u_material;
uniform sampler2D u_texture;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform int u_nBlinn;

void main(){
	vec3 color = texture(u_texture, fs_in.v_texCoords).rgb;

	// ambient
	vec3 ambient = 0.05 * color;
	// diffuse
	vec3 lightDir = normalize(u_lightPos - fs_in.v_fragPos);
	vec3 normal = normalize(fs_in.v_normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(u_viewPos - fs_in.v_fragPos);
	float spec = 0.0f;
	if (u_nBlinn == 1){
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	}
	else{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);
	}
	vec3 specular = vec3(0.3) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}