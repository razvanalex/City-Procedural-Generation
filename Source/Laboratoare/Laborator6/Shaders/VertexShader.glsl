#version 410  

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texCoordinate;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// TODO: output values to fragment
out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_position;
out vec2 frag_texCoordinate;

void main()
{
	// TODO: send output to fragment shader
	frag_color = v_color;
	frag_normal = v_normal;	
	frag_position = v_position;	
	frag_texCoordinate = v_texCoordinate;	

	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
