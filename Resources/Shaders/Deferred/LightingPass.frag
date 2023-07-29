$input v_texcoord0;

SAMPLER2D(gPosition, 0);
SAMPLER2D(gNormal, 1);
SAMPLER2D(gAlbedoSpec, 2);

void main()
{
    vec3 FragPos = texture2D(gPosition, v_texcoord0).rgb;
    vec3 Normal = texture2D(gNormal, v_texcoord0).rgb;
    vec3 Diffuse = texture2D(gAlbedoSpec, v_texcoord0).rgb;
    float Specular = texture2D(gAlbedoSpec, v_texcoord0).r;

    gl_FragColor = vec4(FragPos, 1.0);
}