$input v_color0

#include <bgfx_shader.sh>

void main()
{
	gl_FragColor = vec4(v_color0, 1.0);
}

/*
$input v_texcoord0, v_color0

#include <bgfx_shader.sh>

SAMPLER2D(s_texNormal, 0);
uniform vec4 lightColor;

void main()
{
	//gl_FragColor = texture2D(s_texNormal, v_texcoord0);
	//gl_FragColor = mul(texture2D(s_texNormal, v_texcoord0), lightColor);
	//gl_FragColor = vec4(v_color0, 1.0);
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
*/