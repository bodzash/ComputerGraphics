$input a_position
$output v_texcoord0

uniform mat4 u_ProjView;

void main()
{
    gl_Position =  mul(u_ProjView, vec4(a_position.xyz, 1.0)).xyww;
    v_texcoord0 = a_position;
}