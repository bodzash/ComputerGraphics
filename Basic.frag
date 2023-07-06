$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);
uniform vec4 u_LightColor;

void main()
{
	//gl_FragColor = texture2D(s_texNormal, v_texcoord0) * u_LightColor;
	vec4 lc = vec4(2, 2, 2, 1);
	gl_FragColor = texture2D(s_texNormal, v_texcoord0) * lc;
}