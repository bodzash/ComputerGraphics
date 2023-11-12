$input a_position, a_normal, a_texcoord0, a_indices, a_weight
$output v_texcoord0

#include <bgfx_shader.sh>

#define MAX_BONES 50

uniform mat4 u_ProjView;
uniform mat4 u_Model;
uniform mat4 u_Bones[MAX_BONES];

void main()
{
    /*
	mat4 boneTransform = u_Bones[(int)a_indices[0]] * a_weight[0];
	boneTransform 	  += u_Bones[(int)a_indices[1]] * a_weight[1];
	boneTransform 	  += u_Bones[(int)a_indices[2]] * a_weight[2];
	boneTransform 	  += u_Bones[(int)a_indices[3]] * a_weight[3];

	gl_Position = mul(mul(u_ProjView, u_Model), mul(boneTransform, vec4(a_position, 1.0)));
	*/

    vec4 totalPosition = vec4(0.f, 0.f, 0.f, 0.f);
    for(int i = 0 ; i < 4 ; i++)
    {
        if((int)a_indices[i] == -1) 
            continue;
        if((int)a_indices[i] >= 50) 
        {
            totalPosition = vec4(a_position, 1.0f);
            break;
        }
        vec4 localPosition = mul(u_Bones[(int)a_indices[i]], vec4(a_position,1.0f));
        totalPosition += localPosition * a_weight[i];
    }

	//gl_Position = mul(mul(u_ProjView, u_Model), vec4(a_position, 1.0));
	gl_Position = mul(totalPosition, mul(u_ProjView, u_Model));
	v_texcoord0 = a_texcoord0;
}