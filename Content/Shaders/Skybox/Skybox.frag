$input v_texcoord0

#include <bgfx_shader.sh>

//SAMPLER3D(s_Diffuse, 0);
SAMPLERCUBE(s_Diffuse, 0);

void main()
{
    //gl_FragColor = texture3D(s_Diffuse, v_texcoord0);
    gl_FragColor = textureCube(s_Diffuse, v_texcoord0);
}