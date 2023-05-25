#include <bgfx_shader.sh>

uniform vec4 lightColor;

void main()
{
	gl_FragColor = lightColor;
}