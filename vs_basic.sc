$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_scale;

void main()
{
	gl_Position = vec4(a_position.x * u_scale.x, a_position.y * u_scale.x, a_position.z * u_scale.x, 1.0);
	v_color0 = a_color0;
	v_texcoord0 = a_texcoord0;
}