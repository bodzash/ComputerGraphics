$input a_position, a_normal, a_texcoord0, a_indices, a_weight
$output v_texcoord0

#include <bgfx_shader.sh>

#define MAX_BONES 50

uniform mat4 u_ProjView;
uniform mat4 u_Model;
uniform mat4 u_Bones[MAX_BONES];

void main()
{
	mat4 boneTransform = u_Bones[a_indices[0]] * a_weight[0];
	boneTransform 	  += u_Bones[a_indices[1]] * a_weight[1];
	boneTransform 	  += u_Bones[a_indices[2]] * a_weight[2];
	boneTransform 	  += u_Bones[a_indices[3]] * a_weight[3];
    
	gl_Position = mul(mul(u_ProjView, u_Model), mul(boneTransform, vec4(a_position, 1.0)));
	v_texcoord0 = a_texcoord0;
}