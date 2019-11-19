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
uniform int u_bGamma;

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
	vec3 specular = vec3(0.3) * spec;

	vec3 rscolor = ambient + diffuse + specular;
	if (u_bGamma == 1){
		rscolor = pow(rscolor, vec3(1.0 / 2.2));
	}

    FragColor = vec4(rscolor, 1.0f);
}