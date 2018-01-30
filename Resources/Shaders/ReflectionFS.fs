precision mediump float;

uniform samplerCube		 u_samplerCubeMap;
uniform				vec3 u_cameraPos;	// the camera position is already in world space

varying				vec3 v_normW;
varying				vec3 v_posW;

void main()
{
	vec3 toEye = u_cameraPos - v_posW;
	vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normW));
	gl_FragColor = textureCube(u_samplerCubeMap, reflectDir);
}