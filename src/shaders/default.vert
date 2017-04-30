R"zzz(
#version 330 core

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 light_position;
uniform vec3 camera_position;

in vec4 vertex_position;
in vec4 vertex_normal;
in vec3 vertex_color;

out vec3 color;
out vec4 normal;
out vec4 light_direction;
out vec4 camera_direction;

void main() 
{
	gl_Position = projection * view * model * vertex_position;
	light_direction = light_position - gl_Position;
	camera_direction = vec4(camera_position, 1.0) - gl_Position;
	normal = vertex_normal;
	color = vertex_color;
}
)zzz"
