#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform int spotlight;

void main()
{
	vec3 N = normalize(mat3(Model) * world_normal);
	vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(light_position - world_position);

	// Spot light
	float cut_off = radians(30);
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);

	float ambient_light, attenuation, diffuse_light, specular_light;
	
	// TODO: define ambient light component
	ambient_light = 0.25;

	if (spotlight == 0 || spot_light > cos(cut_off)) {
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);

		// TODO: compute diffuse light component
		diffuse_light = material_kd * max(dot(L, N), 0);

		// TODO: compute specular light component
		specular_light = 0;

		if (diffuse_light > 0)
		{
			vec3 R = reflect(V, N);
			specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);
		}

		attenuation = light_att_factor;
	}

	if (spotlight == 0) {
		float dist = distance(light_position, world_position);
		attenuation = 1.0 / (dist * dist);
	}

	// TODO: compute light
	float light = ambient_light + attenuation * (diffuse_light + specular_light);
	vec3 color = light * object_color;

	// TODO: write pixel out color
	out_color = vec4(color, 1);
}