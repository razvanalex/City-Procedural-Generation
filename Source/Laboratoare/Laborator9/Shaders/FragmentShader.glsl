#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;

uniform int t2;
uniform float time;
uniform int p;


in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec2 texpos = texcoord;
	if (p == 1)
	{
		texpos += vec2(1, 0) * time;
	}

	vec4 color1 = texture2D(texture_1, texpos);
	vec4 color2 = texture2D(texture_2, texcoord);

	if (color1.a <= 0.5f) {
		discard;
	}

	if (t2 == 1)
		out_color = mix(color1, color2, 0.5f);
	else 
		out_color = color1;
}