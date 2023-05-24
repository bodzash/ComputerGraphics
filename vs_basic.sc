$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_scale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
	//gl_Position = mul(proj * view * model, vec4(a_position, 1.0));
	v_color0 = a_color0;
	v_texcoord0 = a_texcoord0;
}

//gl_Position = mul(proj * view * model, vec4(a_position, 1.0));