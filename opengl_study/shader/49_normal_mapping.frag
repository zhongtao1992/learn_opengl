#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
	vec3 v_tangentLightPos;
	vec3 v_tangentFragPos;
	vec3 v_tangentViewPos;
} fs_in;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

void main(){
	vec3 color = texture(u_diffuseMap, fs_in.v_texCoords).rgb;

	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	vec3 lightDir = normalize(fs_in.v_tangentLightPos - fs_in.v_tangentFragPos);
	vec3 normal = texture(u_normalMap, fs_in.v_texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0); // 
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(fs_in.v_tangentViewPos - fs_in.v_tangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.2) * spec;

	vec3 rscolor = ambient + diffuse + specular;

    FragColor = vec4(rscolor, 1.0f);
}