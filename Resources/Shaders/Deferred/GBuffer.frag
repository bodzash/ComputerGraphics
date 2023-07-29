$input v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

SAMPLER2D(s_Diffuse, 0);
SAMPLER2D(s_Specular, 1);

void main()
{
    gl_FragData[0] = v_fragPosition;
    gl_FragData[1] = normalize(v_normal);
    gl_FragData[2].rgb = texture2D(s_Diffuse, v_fragPosition).rgb;
    gl_FragData[2].a = texture2D(s_Specular, v_fragPosition).r;
    // gl_FragData[3] = depth? lol i have no fucking idea
}