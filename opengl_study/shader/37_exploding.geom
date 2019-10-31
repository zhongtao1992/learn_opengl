#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float u_time;
in VS_OUT{
	vec2 texCoords;
} gs_in[];
out vec2 v_texCoords;

vec3 getNormal(){
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal){
	float magnitude = 2.0;
	vec3 direction = normal * ((sin(u_time) + 1.0f) / 2.0f) * magnitude;
	return position + vec4(direction, 1.0f);
}

void main(){
	vec3 normal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	v_texCoords = gs_in[0].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	v_texCoords = gs_in[1].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	v_texCoords = gs_in[2].texCoords;
	EmitVertex();

	EndPrimitive();
}