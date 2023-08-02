$input v_texcoord0

#include <bgfx_shader.sh>

// gPosition
// gNormal
// gAlbedoSpec

SAMPLER2D(s_Diffuse, 0);
SAMPLER2D(s_Specular, 1);
SAMPLER2D(s_Normal, 2);

uniform vec4 u_ViewPosition;
uniform vec4 u_DirLight[4];

#define DirLight_Direction u_DirLight[0].xyz
#define DirLight_Ambient u_DirLight[1].xyz
#define DirLight_Diffuse u_DirLight[2].xyz
#define DirLight_Specular u_DirLight[3].xyz

void main()
{
    /*
    vec3 FragPos = texture2D(s_Diffuse, v_texcoord0).rgb;
    vec3 Normal = texture2D(s_Specular, v_texcoord0).rgb;
    vec3 Albedo = texture2D(s_Normal, v_texcoord0).rgb;
    float Specular = texture2D(s_Normal, v_texcoord0).r;
    
    
    // Sum shit
    vec3 ViewDir = normalize(u_ViewPosition.rgb - FragPos);

    // Ambient
    vec3 FinalColor = Albedo * 0.8;

    // Diffuse
    vec3 LightDir = normalize(-DirLight_Direction);
    float Diff = max(dot(normalize(Normal), LightDir), 0.0);
    vec3 Diffuse = DirLight_Diffuse * Diff * Albedo;

    //gl_FragColor = vec4(FinalColor, 1.0);
    //gl_FragColor = vec4(Albedo, 1.0);
    */

    //gl_FragColor = vec4(FragPos, 1.0);
    gl_FragColor = texture2D(s_Diffuse, v_texcoord0);
}