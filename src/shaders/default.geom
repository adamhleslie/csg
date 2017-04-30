R"zzz(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

in vec4 vs_light_direction[];
in vec4 vs_camera_direction[];
in vec4 vs_normal[];

out vec4 face_normal;
out vec4 light_direction;
out vec4 camera_direction;
out vec4 world_position;
out vec4 vertex_normal;

void main() 
{
	vec3 a = gl_in[0].gl_Position.xyz;
	vec3 b = gl_in[1].gl_Position.xyz;
	vec3 c = gl_in[2].gl_Position.xyz;
	vec3 u = normalize(b - a);
	vec3 v = normalize(c - a);
	face_normal = normalize(vec4(normalize(cross(u, v)), 0.0));

	int n = 0;
	for (n = 0; n < gl_in.length(); n++) {
		light_direction = normalize(vs_light_direction[n]);
		camera_direction = normalize(vs_camera_direction[n]);
		world_position = gl_in[n].gl_Position;
		vertex_normal = vs_normal[n];
		gl_Position = projection * view * model * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz"
