$input v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

SAMPLER2D(s_Diffuse, 0);
SAMPLER2D(s_Specular, 1);

void main()
{
    gl_FragData[0] = vec4(v_fragPosition, 1.0);
    gl_FragData[1] = vec4(normalize(v_normal), 1.0);
    gl_FragData[2] = vec4(texture2D(s_Diffuse, v_texcoord0).rgb, texture2D(s_Specular, v_texcoord0).r);
}