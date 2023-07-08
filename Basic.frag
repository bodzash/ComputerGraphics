$input v_color0, v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

SAMPLER2D(s_Albedo, 0);
SAMPLER2D(s_Specular, 1);
//SAMPLER2D(s_Normal, 2);
//SAMPLER2D(s_Emission, 3);
uniform vec4 u_ViewPosition;
uniform vec4 u_Material;
uniform vec4 u_Light[4];
uniform vec4 u_DirLight[4];
uniform vec4 u_PointLight[5];

struct Material
{
    vec3 Diffuse;
    vec3 Specular;
    //vec3 Normal;
    float Shininess;
};

struct Light
{
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct DirectionalLight
{
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct PointLight
{
    vec3 Position;  
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Constant;
    float Linear;
    float Quadratic;
};

vec3 CalcDirectionalLighting(DirectionalLight light, Material material, vec3 v_normal, vec3 v_fragPosition);
vec3 CalcPointLighting(PointLight light, Material material, vec3 v_normal, vec3 v_fragPosition);

void main()
{
    // Material
    Material material;
    material.Diffuse = texture2D(s_Albedo, v_texcoord0).xyz;
    material.Specular = texture2D(s_Specular, v_texcoord0).xyz;
    material.Shininess = u_Material.x;

    // Directional Light
    DirectionalLight light;
    light.Direction = u_Light[0].xyz;
    light.Ambient = u_Light[1].xyz;
    light.Diffuse = u_Light[2].xyz;
    light.Specular = u_Light[3].xyz;

    // Point Light
    PointLight plight;
    plight.Position = u_PointLight[0].xyz;
    plight.Ambient = u_PointLight[1].xyz;
    plight.Diffuse = u_PointLight[2].xyz;
    plight.Specular = u_PointLight[3].xyz;
    plight.Constant = u_PointLight[4].x;
    plight.Linear = u_PointLight[4].y;
    plight.Quadratic = u_PointLight[4].z;

    //gl_FragColor = vec4(CalcDirectionalLighting(light, material, v_normal, v_fragPosition), 1.0);
    gl_FragColor = vec4(CalcPointLighting(plight, material, v_normal, v_fragPosition), 1.0);
}

vec3 CalcDirectionalLighting(DirectionalLight light, Material material, vec3 v_normal, vec3 v_fragPosition)
{
    // Ambient lighting
    vec3 ambient = light.Ambient * material.Diffuse;

	// Diffuse lighting
	vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * diff * material.Diffuse;

	// Specular lighting
    vec3 viewDir = normalize(u_ViewPosition.xyz - v_fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = light.Specular * spec * material.Specular;

    return ambient + diffuse + specular;
}

vec3 CalcPointLighting(PointLight light, Material material, vec3 v_normal, vec3 v_fragPosition)
{
    // Ambient lighting
    vec3 ambient = light.Ambient * material.Diffuse;
  	
    // Diffuse lighting
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light.Position - v_fragPosition);
    float diff = max(dot(v_normal, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * diff * material.Diffuse;  
    
    // Specular lighting
    vec3 viewDir = normalize(u_ViewPosition.xyz - v_fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = light.Specular * spec * material.Specular;  
    
    // Attenuation
    float distance = length(light.Position - v_fragPosition);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;   
        
    return ambient + diffuse + specular;
}

// Basic lighting
/*
    // Material
    Material material;
    material.Diffuse = texture2D(s_Albedo, v_texcoord0);
    material.Specular = texture2D(s_Specular, v_texcoord0);
    material.Shininess = u_Material.x;

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
    vec3 specular = light.Specular * spec * material.Specular;
	
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
*/