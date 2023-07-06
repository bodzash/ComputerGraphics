$input v_color0, v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);
uniform vec4 u_LightColor;
uniform vec4 u_LightPosition;

void main()
{
	// Ambient lighting
	float ambientStrength = 0.1; // 0.1
    vec4 ambient = ambientStrength * u_LightColor;

	// Diffuse lighting
	vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_LightPosition - v_fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * u_LightColor;

	gl_FragColor = texture2D(s_texNormal, v_texcoord0) * (ambient + diffuse);
	//gl_FragColor = texture2D(s_texNormal, v_texcoord0) * Ambient;
}