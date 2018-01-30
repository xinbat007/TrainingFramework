attribute	vec3 a_posL;
uniform		mat4 u_mvpMatrix;

varying		vec4 v_pos;

void main(void)
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_mvpMatrix * posL;
	//gl_Position.z = 1.0 - 0.0001;
	v_pos = posL;
}