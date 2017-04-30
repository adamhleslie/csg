R"zzz(
#version 330 core

uniform float alpha;

smooth in vec3 color;
smooth in vec4 normal;
smooth in vec4 light_direction;
smooth in vec4 camera_direction;

out vec4 fragment_color;

void main() 
{
	fragment_color = vec4(normalize(normal.xyz), alpha);
}
)zzz"