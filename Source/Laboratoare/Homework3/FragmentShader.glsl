#version 330

// Texture
uniform sampler2D texture;
uniform int textureEnable;


// get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;


// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 color;


// Spot light
#define MAX_LIGHTS 4
uniform int spotLight;
uniform vec3 positions[MAX_LIGHTS];
uniform float coneAngles[MAX_LIGHTS];
uniform vec3 coneDirs[MAX_LIGHTS];
uniform vec3 colors[MAX_LIGHTS];


layout(location = 0) out vec4 out_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()

{
	vec3 N = world_normal;
	vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(-light_direction);

	float ambient_light, diffuse_light, specular_light;
	
	// Define ambient light component
	ambient_light = 0.25;

	// Compute diffuse light component
	diffuse_light = material_kd * max(dot(L, N), 0);

	// Compute specular light component
	specular_light = 0;
	
	if (diffuse_light > 0)
	{
		vec3 R = -reflect(L, N);
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	// Compute light
	float light = ambient_light + diffuse_light + specular_light;
	vec3 final_color;

	// Apply texture, if required
	if (textureEnable == 1 && texcoord != vec2(-1.0f)) 
	{
		vec4 texColor = texture2D(texture, texcoord);
		if (texColor.a <= 0.5f) {
			discard;
		}
		final_color = light * texColor.rgb;

	} else {
		final_color = light * color;
	}

	// Add spotLight
	if (spotLight == 1) {
		for (int i = 0; i < MAX_LIGHTS; i++) {
			vec3 sL = normalize(positions[i] - world_position);
			 
			float cut_off = radians(coneAngles[i]);
			float spot_light = dot(-sL, coneDirs[i]);
			float spot_light_limit = cos(cut_off);
			float spotColor = 0;
			float attenuation = 0;

			if (spot_light > cos(cut_off)) {
				float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
				float light_att_factor = pow(linear_att, 2);
				 
				spotColor = 0.5 * max(dot(sL, N), 0);
				attenuation = light_att_factor;
			}

			final_color += attenuation * spotColor * colors[i];
		}
	}

	// Write pixel out color
	out_color = vec4(final_color, 1);
}