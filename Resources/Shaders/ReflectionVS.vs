attribute			vec3 a_posL;
attribute			vec3 a_normL;
uniform mediump		mat4 u_mvpMatrix;
uniform				mat4 u_maWorld;	// world matrix of the model

varying				vec3 v_normW;
varying				vec3 v_posW;

void main()
{
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
	// 
	v_normW = (u_maWorld * vec4(a_normL, 0.0)).xyz;
	//
	v_posW  = (u_maWorld * vec4(a_posL, 1.0)).xyz;
}