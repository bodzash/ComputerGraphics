$input v_color0, v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);
uniform vec4 u_LightColor;
uniform vec4 u_LightPosition;
uniform vec4 u_ViewPosition;

void main()
{
	// Ambient lighting
	float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * u_LightColor;

	// Diffuse lighting
	vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_LightPosition - v_fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * u_LightColor;

	// Specular lighting
	/*
	float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;
	*/
	float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(u_LightColor.x, u_LightColor.y, u_LightColor.z);
	
	//gl_FragColor = texture2D(s_texNormal, v_texcoord0) * (ambient + diffuse);
	gl_FragColor = texture2D(s_texNormal, v_texcoord0) * vec4(ambient + diffuse + specular, 1.0);
}