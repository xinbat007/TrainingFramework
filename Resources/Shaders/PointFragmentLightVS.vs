uniform		mediump mat4 u_mvpMatrix;
uniform		mediump mat4 u_maModel;

attribute	vec3 a_posL;
attribute	vec3 a_normL;
attribute	vec2 a_uv;

varying		vec4 v_vertexW;
varying		vec4 v_normalW;
varying		vec2 v_uv;

void main()
{
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
	
	v_vertexW = u_maModel * vec4(a_posL, 1.0);
	v_normalW = normalize(u_maModel * vec4(a_normL, 0.0));
	v_uv = a_uv;
}