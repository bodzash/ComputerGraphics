$input a_position
//$output v_position

#include <bgfx_shader.sh>

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	gl_Position = mul(mul(camMatrix, model), vec4(a_position, 1.0));
	//gl_Position = mul(camMatrix * model, vec4(a_position, 1.0));
}