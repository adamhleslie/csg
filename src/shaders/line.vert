R"zzz(
#version 330 core
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
in vec4 vertex_position;

void main() {
	gl_Position = projection * view * model * vertex_position;
}
)zzz"