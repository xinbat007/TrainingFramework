uniform   mat4 u_mvpMatrix;
attribute vec3 a_posL;
//attribute vec4 a_color;
//--------------
//varying   vec4 v_color;

void main()
{
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
	//vec4 posL = vec4(a_posL, 1.0);
	//gl_Position = vec4(a_posL, 1.0);
	//v_color = a_color;
}