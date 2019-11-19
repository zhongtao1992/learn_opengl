#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec2 v_texCoords;
	vec3 v_fragPos;
	vec3 v_normal;
} fs_in;

struct Light{
	vec3 Position;
	vec3 Color;
};

uniform Light u_lights[4];
uniform sampler2D u_texture;

void main(){
	vec3 color = texture(u_texture, fs_in.v_texCoords).rgb;
    vec3 normal = normalize(fs_in.v_normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; i++)
    {
        // diffuse
        vec3 lightDir = normalize(u_lights[i].Position - fs_in.v_fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = u_lights[i].Color * diff * color;      
        vec3 result = diffuse;        
        // attenuation 
        float distance = length(fs_in.v_fragPos - u_lights[i].Position);
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    vec3 result = ambient + lighting;
    FragColor = vec4(result, 1.0);
}