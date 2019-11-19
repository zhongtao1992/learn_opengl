#version 330 core

out vec4 FragColor;

in vec2 v_texCoords;

uniform sampler2D u_scene;
uniform sampler2D u_bloomBlur;
uniform bool u_bloom;
uniform float u_exposure;

void main(){
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_scene, v_texCoords).rgb;
    vec3 bloomColor = texture(u_bloomBlur, v_texCoords).rgb;

    if (u_bloom){
        hdrColor += bloomColor;
    }
    vec3 result = vec3(1.0) - exp(-hdrColor * u_exposure);
    result = pow(result, vec3(1.0) / gamma);

    FragColor = vec4(result, 1.0);
}