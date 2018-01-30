uniform		mediump mat4 u_mvpMatrix;
attribute	vec3 a_posL;

varying		vec2 v_uv;

void main()
{
	gl_Position = vec4(a_posL, 1.0);
	v_uv = (a_posL.xy + vec2(1.0, 1.0)) * 0.5;
}