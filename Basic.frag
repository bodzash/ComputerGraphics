$input v_color0, v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

SAMPLER2D(s_Albedo, 0);
uniform vec4 u_ViewPosition;
uniform vec4 u_Material[3];
uniform vec4 u_Light[4];

struct Material
{
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct Light {
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

void main()
{
    // Material
    Material material;
    //material.Diffuse = u_Material[0].xyz;
    material.Diffuse = texture2D(s_Albedo, v_texcoord0);
    material.Specular = u_Material[1].xyz;
    material.Shininess = u_Material[2].x;

    // Light
    Light light;
    light.Position = u_Light[0].xyz;
    light.Ambient = u_Light[1].xyz;
    light.Diffuse = u_Light[2].xyz;
    light.Specular = u_Light[3].xyz;

	// Ambient lighting
    vec3 ambient = light.Ambient * material.Diffuse;

	// Diffuse lighting
	vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light.Position - v_fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * diff * material.Diffuse;

	// Specular lighting
	float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = light.Specular * (spec * material.Specular);
	
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
    //gl_FragColor = texture2D(s_Albedo, v_texcoord0) * vec4(ambient + diffuse + specular, 1.0);
}