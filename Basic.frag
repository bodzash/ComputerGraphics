$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);
uniform vec4 lightColor;

void main()
{
	gl_FragColor = texture2D(s_texNormal, v_texcoord0);
	//gl_FragColor = mul(texture2D(s_texNormal, v_texcoord0), lightColor);
}