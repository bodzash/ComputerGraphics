$input a_position, a_color0, a_normal, a_texcoord0
$output v_color0, v_texcoord0, v_normal, v_fragPosition

#include <bgfx_shader.sh>

uniform mat4 camMatrix;
uniform mat4 u_Model;
uniform mat4 u_InverseModel;

void main()
{
	gl_Position = mul(mul(camMatrix, u_Model), vec4(a_position, 1.0));
	v_color0 = a_color0;
	v_texcoord0 = a_texcoord0;
	// v_normal = a_normal;
	//v_normal = mat3(transpose((u_InverseModel)) * a_normal;
	v_normal = mul(transpose(u_InverseModel), a_normal);
	vec3 shit = mul(u_Model, vec4(a_position, 1.0));
	v_fragPosition = shit;
}