#version 410  

// TODO: get values from fragment shader
in vec3 frag_normal;
in vec3 frag_color;
in vec3 frag_position;
in vec2 frag_texCoordinate;

uniform float time;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	vec3 color = vec3(sin(time), cos(time), 0);	
	out_color = vec4(color, 1);
}