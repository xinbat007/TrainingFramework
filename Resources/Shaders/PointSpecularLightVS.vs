uniform		mediump mat4 u_mvpMatrix;
uniform		mediump mat4 u_maModel;

uniform		vec3 u_cameraPos;
uniform		vec4 light_position;
uniform		vec4 light_ambient;
uniform		vec4 light_diffuse;
uniform		vec4 light_specular;
uniform		float light_constantAttenuation;
uniform		float light_linearAttenuation;
uniform		float light_quadraticAttenuation;
uniform		float material_shininess;

attribute	vec3 a_posL;
attribute	vec3 a_normL;
attribute	vec2 a_uv;

varying		vec4 v_ambient;
varying		vec4 v_diffuse;
varying		vec4 v_specular;
varying		vec2 v_uv;

void main()
{
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
	v_uv = a_uv;
	
	vec4 currentVertexPosW	= (u_maModel * vec4(a_posL, 1.0));
	vec4 currentLightVector	= normalize(light_position - currentVertexPosW);
	vec4 normalVectorW		= normalize(u_maModel * vec4(a_normL, 0.0));
	vec4 eyeVectorW			= normalize(vec4(u_cameraPos, 1.0) - currentVertexPosW);
	vec4 reflectVector		= normalize(-1.0 * reflect(currentLightVector, normalVectorW));
	
	float distance		= length(light_position - currentVertexPosW);	
	float attenuation	= 1.0 /(light_constantAttenuation +
								light_linearAttenuation * distance + 
								light_quadraticAttenuation * distance * distance);
	float NDotL			= max(0.0, dot(normalVectorW, currentLightVector));
	float RDotE			= max(0.0, dot(eyeVectorW, reflectVector));
	float reflectPower	= pow(RDotE, material_shininess);
	
	v_ambient = light_ambient;
	v_diffuse = light_diffuse * NDotL * attenuation;
	v_specular = clamp(light_specular * reflectPower, 0.0, 1.0) * attenuation;
}