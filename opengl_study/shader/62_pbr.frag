#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
	vec3 v_worldPos;
} fs_in;

// material 
uniform vec3 u_albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;

// IBL
uniform samplerCube u_irradianceMap;
uniform samplerCube u_prefilterMap;
uniform sampler2D u_brdfLUT;

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

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){
	vec3 N = normalize(fs_in.v_normal);
	vec3 V = normalize(u_camPos - fs_in.v_worldPos);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, u_albedo, u_metallic);

	vec3 Lo = vec3(0.0);
	for (int i = 0; i < 4; ++i){
		vec3 L = normalize(u_lightPositions[i] - fs_in.v_worldPos);
		vec3 H = normalize(V + L);
		float distance = length(u_lightPositions[i] - fs_in.v_worldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_lightColors[i] * attenuation;

		float NDF = distributionGGX(N, H, u_roughness);
		float G = geometrySmith(N, V, L, u_roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = nominator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - u_metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * u_albedo / PI + specular) * radiance * NdotL;
	}

	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, u_roughness);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - u_metallic;

	vec3 irradiance = texture(u_irradianceMap, N).rgb;
	vec3 diffuse = irradiance * u_albedo;

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(u_prefilterMap, R, u_roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(u_brdfLUT, vec2(max(dot(N, V), 0.0), u_roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient = (kD * diffuse + specular) * u_ao;
	vec3 color = ambient + Lo;

	// hdr tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}