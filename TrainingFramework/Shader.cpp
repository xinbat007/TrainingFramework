#include <stdafx.h>
#include "Shader.h"
#include <string>

GLuint Shader::LoadShader ( GLenum type, char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;

	// Load the shader source
	FILE * pf;
	pf = fopen(filename, "rb" );
	if (pf == 0)
	{
		LOGI("ERROR: COULD NOT open file: %s", filename);
		return NULL;
	}
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **)&shaderSrc, NULL );
	delete [] shaderSrc;

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			LOGI( "Error compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

GLuint Shader::LoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );   

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}

int Shader::Load(char* fileVertexShader, char* fileFragmentShader)
{
	char vs[200], fs[200];
#ifdef WIN32
	sprintf(vs, "%s", fileVertexShader);
	sprintf(fs, "%s", fileFragmentShader);
#else
	sprintf(vs, "%s/Resources/%s", SD_FOLDER, fileVertexShader);
	sprintf(fs, "%s/Resources/%s", SD_FOLDER, fileFragmentShader);
#endif
	m_VSFile = new char[strlen(vs)];
	m_FSFile = new char[strlen(fs)];
	strcpy(m_VSFile, vs);
	strcpy(m_FSFile, fs);
	LOGI("Load shader: %s, %s", m_VSFile, m_FSFile);
	//---------------------------------
	vertexShader = LoadShader(GL_VERTEX_SHADER, m_VSFile);
	if ( vertexShader == 0 )
	{
		LOGI("ERROR: COULD NOT load shader VS");
		return -1;
	}
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, m_FSFile);
	if ( fragmentShader == 0 )
	{
		LOGI("ERROR: COULD NOT load shader FS");
		glDeleteShader( vertexShader );
		return -2;
	}
	program = LoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute	= glGetAttribLocation (program, "a_posL");
	uiTextureAttribute	= glGetAttribLocation (program, "a_uv");
	mvpAttribute		= glGetUniformLocation(program, "u_mvpMatrix");
	colorAttribute		= glGetUniformLocation(program, "u_color");
	// Sky
	m_cubeUniform		= glGetUniformLocation(program, "u_samplerCubeMap");
	// Multi texture
	m_locBlendMap		= glGetUniformLocation(program, "u_BlendMap");
	m_locTexture1		= glGetUniformLocation(program, "u_Texture1");
	m_locTexture2		= glGetUniformLocation(program, "u_Texture2");
	m_locTexture3		= glGetUniformLocation(program, "u_Texture3");
	// Fog
	m_cameraPos			= glGetUniformLocation(program, "u_cameraPos");
	m_fogStart			= glGetUniformLocation(program, "u_fogStart");
	m_fogLength			= glGetUniformLocation(program, "u_fogLength");
	// Fire
	m_time				= glGetUniformLocation(program, "u_Time");
	// Reflection
	m_normL				= glGetAttribLocation (program, "a_normL");
	m_worldMatrix		= glGetUniformLocation(program, "u_maWorld");
	// Direction Lighting
	m_maModel			= glGetUniformLocation(program, "u_maModel");
	m_lightPosition		= glGetUniformLocation(program, "light_position");
	m_lightAmbient		= glGetUniformLocation(program, "light_ambient");
	m_lightDiffuse		= glGetUniformLocation(program, "light_diffuse");
	// Point Lighting
	m_maViewModel		= glGetUniformLocation(program, "u_maViewModel");
	m_lightConstantAtn	= glGetUniformLocation(program, "light_constantAttenuation");
	m_lightLinearAtn	= glGetUniformLocation(program, "light_linearAttenuation");
	m_lightQuadraticAtn	= glGetUniformLocation(program, "light_quadraticAttenuation");
	// Point Specular Lighting
	m_lightSpecular		= glGetUniformLocation(program, "light_specular");
	m_materialShininess = glGetUniformLocation(program, "material_shininess");
	// Point Fragment Lighting
	m_ambientWeight		= glGetUniformLocation(program, "u_ambientWeight");
	// Normal Mapping
	m_tangentL			= glGetAttribLocation (program, "a_tangentL");
	m_bitangentL		= glGetAttribLocation (program, "a_bitangentL");
	m_textureNormalMap	= glGetUniformLocation(program, "u_normalMap");
	// Blur
	m_step				= glGetUniformLocation(program, "u_step");
	// Bloom
	m_limit				= glGetUniformLocation(program, "u_limit");
	// DOF
	m_near				= glGetUniformLocation(program, "u_near");
	m_far				= glGetUniformLocation(program, "u_far");
	m_fade				= glGetUniformLocation(program, "u_fade");
	m_clarity			= glGetUniformLocation(program, "u_clarity");
	
	return 0;
}

void Shader::Log()
{
	LOGI("Shader %d: %d, %s, %s", m_id, m_result, m_VSFile, m_FSFile);
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	SAFE_DEL_ARRAY(m_VSFile);
	SAFE_DEL_ARRAY(m_FSFile);
}