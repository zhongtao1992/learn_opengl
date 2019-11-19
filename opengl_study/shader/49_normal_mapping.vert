#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

out VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
	vec3 v_tangentLightPos;
	vec3 v_tangentFragPos;
	vec3 v_tangentViewPos;
} vs_out;

void main(){
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);

    vs_out.v_fragPos = vec3(u_model * vec4(aPos, 1.0));
    vs_out.v_texCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(u_model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.v_tangentLightPos = TBN * u_lightPos;
    vs_out.v_tangentFragPos = TBN * vs_out.v_fragPos;
    vs_out.v_tangentViewPos = TBN * u_viewPos;
}