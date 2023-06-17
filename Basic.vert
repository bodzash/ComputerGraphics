$input a_position, a_texcoord0
$output v_texcoord0

#include <bgfx_shader.sh>

uniform mat4 camMatrix;

void main()
{
	gl_Position = mul(camMatrix, vec4(a_position, 1.0));
	v_texcoord0 = a_texcoord0;
}