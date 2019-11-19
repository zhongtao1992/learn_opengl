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
uniform sampler2D u_parallaxMap;
uniform float u_heightScale;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir){
	float minLayers = 8.0f;
	float maxLayers = 32.0f;
	float numLayers = mix(minLayers, maxLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0f;
	vec2 P = viewDir.xy * u_heightScale;
	vec2 deltaTexCoords = P / numLayers;
	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(u_parallaxMap, currentTexCoords).r;

	while(currentLayerDepth < currentDepthMapValue){
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(u_parallaxMap, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}

	return currentTexCoords;
}

void main(){
	vec3 viewDir = normalize(fs_in.v_tangentViewPos - fs_in.v_tangentFragPos);
	vec2 texCoords = fs_in.v_texCoords;
	texCoords = parallaxMapping(fs_in.v_texCoords, viewDir);

	if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0){
		discard;
	}

	vec3 normal = texture(u_normalMap, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(u_diffuseMap, texCoords).rgb;

	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	vec3 lightDir = normalize(fs_in.v_tangentLightPos - fs_in.v_tangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.2) * spec;

	vec3 rscolor = ambient + diffuse + specular;

    FragColor = vec4(rscolor, 1.0f);
}