precision mediump float;

uniform sampler2D u_Texture1;
uniform sampler2D u_normalMap;

uniform vec3	u_cameraPos;
uniform vec4	light_ambient;
uniform vec4	light_position;
uniform vec4	light_diffuse;
uniform vec4	light_specular;
uniform float	u_ambientWeight;
uniform float	light_constantAttenuation;
uniform float	light_linearAttenuation;
uniform float	light_quadraticAttenuation;
uniform float	material_shininess;

varying vec4 v_vertexW;
varying	vec4 v_normalW;
varying vec4 v_tangentW;
varying vec4 v_bitangentW;
varying vec2 v_uv;

void main()
{
	// Normal Map - Re-calculate vector normal
	vec4 normal			= texture2D(u_normalMap, v_uv);
	mat4 TBN			= mat4(normalize(v_tangentW), normalize(v_bitangentW), normalize(v_normalW), vec4(0.0));
	vec4 normalW		= normalize(TBN * (2.0 * normal - 1.0));
	
	// Phong Lighting
	vec4 ambientComponent	= light_ambient;
	vec4 currentLightVector	= normalize(light_position - v_vertexW);
	vec4 lightDirection		= light_position - v_vertexW;
	float distance			= length(lightDirection);
	float attenuation	= 1.0 /(light_constantAttenuation +
								light_linearAttenuation * distance + 
								light_quadraticAttenuation * distance * distance);
	
	vec4 toEye			= normalize(vec4(u_cameraPos, 1.0) - v_vertexW);
	vec4 reflectVector	= normalize(-reflect(currentLightVector, normalW));
	float NDotL			= max(0.0, dot(normalW, currentLightVector));
	float RDotE			= max(0.0, dot(toEye, reflectVector));
	vec4 diffuseComponent = light_diffuse * NDotL * attenuation;
	
	float reflectPower	= pow(RDotE, material_shininess);
	vec4 specularComponent	= clamp(light_specular * reflectPower, 0.0, 1.0) * attenuation;
	
	// Color
	vec4 colorFragment = texture2D(u_Texture1, v_uv);
	diffuseComponent = mix(diffuseComponent, ambientComponent, u_ambientWeight); // diffuseComponent*(1-u_ambientWeight) + ambientComponent*u_ambientWeight;
	gl_FragColor = vec4((colorFragment * diffuseComponent + specularComponent).xyz, colorFragment.a);
	
}