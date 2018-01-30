uniform		mat4	u_mvpMatrix;
attribute	vec3	a_posL;
attribute	vec2	a_uv;

uniform		vec3	u_cameraPos;
uniform		mat4	u_maModel;

varying		float	v_eyeDist;
varying		vec2	v_uv;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	
	// Calculate distance eyes - pixel
	vec4 vViewPos = u_maModel * posL;
	vec3 d = vec3 (	vViewPos.x - u_cameraPos.x,
					vViewPos.y - u_cameraPos.y,
					vViewPos.z - u_cameraPos.z);
	v_eyeDist = length(d);

	gl_Position = u_mvpMatrix * posL;
	v_uv = a_uv;
}