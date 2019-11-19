#version 330 core

out vec4 FragColor;
in vec2 v_texCoords;

uniform sampler2D u_hdrBuffer;
uniform bool u_hdr;
uniform float u_exposure;

void main(){
    float gamma = 2.2;
    vec3 hdrColor = texture(u_hdrBuffer, v_texCoords).rgb;
    if (u_hdr){
    	vec3 result = vec3(1.0) - exp(-hdrColor * u_exposure);
    	result = pow(result, vec3(1.0 / gamma));
    	FragColor = vec4(result, 1.0);
    }
    else{
    	vec3 result = pow(hdrColor, vec3(1.0 / gamma));
    	FragColor = vec4(result, 1.0);
    }
}