$input a_position, a_normal, a_texcoord0, a_texcoord1, a_texcoord2
$output v_texcoord0

#include <bgfx_shader.sh>

#define MAX_BONES 50

uniform mat4 u_ProjView;
uniform mat4 u_Model;
uniform mat4 u_Bones[MAX_BONES];

void main()
{
	mat4 boneTransform = u_Bones[(int)a_texcoord1.x] * a_texcoord2.x;
	boneTransform += u_Bones[(int)a_texcoord1.y] * a_texcoord2.y;
	boneTransform += u_Bones[(int)a_texcoord1.z] * a_texcoord2.z;
	boneTransform += u_Bones[(int)a_texcoord1.w] * a_texcoord2.w;
	
	gl_Position = mul(mul(u_ProjView, u_Model), mul(boneTransform, vec4(a_position, 1.0)));
	v_texcoord0 = a_texcoord0;
}