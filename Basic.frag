$input v_color0, v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

#define MAX_POINT_LIGHT 4

SAMPLER2D(s_Albedo, 0);
SAMPLER2D(s_Specular, 1);
//SAMPLER2D(s_Normal, 2);
//SAMPLER2D(s_Emission, 3);
uniform vec4 u_ViewPosition;
uniform vec4 u_Material;
uniform vec4 u_DirLight[4];
uniform vec4 u_NumPointLight;
uniform vec4 u_PointLight[MAX_POINT_LIGHT][5];
uniform vec4 u_SpotLight[7];

struct Material
{
    vec3 Diffuse;
    vec3 Specular;
    //vec3 Normal;
    //vec3 Emission;
    float Shininess;
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

    // concat these into a vec3 maybe lol
    float Constant;
    float Linear;
    float Quadratic;
};

struct SpotLight
{
    vec3 Position;
    vec3 Direction;
    // concat these into a vec2 maybe lol
    float CutOff;
    float OuterCutOff;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    // concat these into a vec3 maybe lol
    float Constant;
    float Linear;
    float Quadratic;
};

vec3 CalcDirectionalLighting(DirectionalLight light, Material material, vec3 v_normal, vec3 v_fragPosition);
vec3 CalcPointLighting(PointLight light, Material material, vec3 v_normal, vec3 v_fragPosition);
vec3 CalcSpotLighting(SpotLight light, Material material, vec3 v_normal, vec3 v_fragPosition);

void main()
{
    // Material
    Material material;
    material.Diffuse = texture2D(s_Albedo, v_texcoord0).xyz;
    material.Specular = texture2D(s_Specular, v_texcoord0).xyz;
    material.Shininess = u_Material.x;

    // Directional Light (only one)
    DirectionalLight dlight;
    dlight.Direction = u_DirLight[0].xyz;

    dlight.Ambient = u_DirLight[1].xyz;
    dlight.Diffuse = u_DirLight[2].xyz;
    dlight.Specular = u_DirLight[3].xyz;

    /*
    // One spotlight for now can be more tho
    SpotLight slight;
    slight.Position = u_SpotLight[0].xyz;
    slight.Direction = u_SpotLight[1].xyz;
    slight.CutOff = u_SpotLight[2].x;
    slight.OuterCutOff = u_SpotLight[2].y;

    slight.Ambient = u_SpotLight[3].xyz;
    slight.Diffuse = u_SpotLight[4].xyz;
    slight.Specular = u_SpotLight[5].xyz;

    slight.Constant = u_SpotLight[6].x;
    slight.Linear = u_SpotLight[6].y;
    slight.Quadratic = u_SpotLight[6].z;
    */

    // NOTE: OPTIMIZE: use: #define DirLight_Ambient u_DirLight[1].xyz and so on...
    // Pre-Calculate: norm, viewDir and pass as arguments lol
    // We can keep the (Material struct) material.Diffuse = texture2D(), and shit like those.
    // Man we do a lot of fucking copying when assigning struct members smh
    vec3 result = CalcDirectionalLighting(dlight, material, v_normal, v_fragPosition);

    // Multiple point lights
    /*
    for (int i = 0; i < u_NumPointLight.x; i++)
    {
        PointLight plight;
        plight.Position = u_PointLight[i][0].xyz;

        plight.Ambient = u_PointLight[i][1].xyz;
        plight.Diffuse = u_PointLight[i][2].xyz;
        plight.Specular = u_PointLight[i][3].xyz;

        plight.Constant = u_PointLight[i][4].x;
        plight.Linear = u_PointLight[i][4].y;
        plight.Quadratic = u_PointLight[i][4].z;

        // Add plight
        result += CalcPointLighting(plight, material, v_normal, v_fragPosition);
    }

    // Add slight
    result += CalcSpotLighting(slight, material, v_normal, v_fragPosition);
    */

    gl_FragColor = vec4(result, 1.0);
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

vec3 CalcSpotLighting(SpotLight light, Material material, vec3 v_normal, vec3 v_fragPosition)
{
    // Ambient lighting
    vec3 ambient = light.Ambient * material.Diffuse;
    
    // Diffuse lighting
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light.Position - v_fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * diff * material.Diffuse;
    
    // Specular lighting
    vec3 viewDir = normalize(u_ViewPosition - v_fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = light.Specular * spec * material.Specular;
    
    // Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = (light.CutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // Attenuation
    float distance = length(light.Position - v_fragPosition);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    ambient *= attenuation; 
    diffuse *= attenuation;
    specular*= attenuation;

    return ambient + diffuse + specular;
}