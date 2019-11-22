#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
	vec3 v_worldPos;
} fs_in;

// material 
uniform sampler2D u_albedoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_metallicMap;
uniform sampler2D u_roughnessMap;
uniform sampler2D u_aoMap;

// lights
uniform vec3 u_lightPositions[4];
uniform vec3 u_lightColors[4];

uniform vec3 u_camPos;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001);
}

float geometrySchlickGGX(float NdotV, float roughness){
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){
	vec3 albedo = pow(texture(u_albedoMap, fs_in.v_texCoords).rgb, vec3(2.2));
	float metallic = texture(u_metallicMap, fs_in.v_texCoords).r;
	float roughness = texture(u_roughnessMap, fs_in.v_texCoords).r;
	float ao = texture(u_aoMap, fs_in.v_texCoords).r;

	vec3 N = normalize(fs_in.v_normal);
	vec3 V = normalize(u_camPos - fs_in.v_worldPos);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	for (int i = 0; i < 4; ++i){
		vec3 L = normalize(u_lightPositions[i] - fs_in.v_worldPos);
		vec3 H = normalize(V + L);
		float distance = length(u_lightPositions[i] - fs_in.v_worldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_lightColors[i] * attenuation;

		float NDF = distributionGGX(N, H, roughness);
		float G = geometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	// hdr tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}