$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);

void main()
{
	gl_FragColor = texture2D(s_texNormal, v_texcoord0);
	//gl_FragColor = vec4(v_color0, 1.0);
	//gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}