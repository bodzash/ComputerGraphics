$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);

void main()
{
	gl_FragColor = texture2D(s_texNormal, v_texcoord0);
}