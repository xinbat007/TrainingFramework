#include <stdafx.h>
#include "EffectManager.h"
///////////////////////////////////////////////////
EffectManager* EffectManager::ms_pInstance = NULL;
///////////////////////////////////////////////////
EffectManager::EffectManager()
{
	m_shaders		 = NULL;
	m_effects		 = NULL;
	m_FBOId			 = NULL;
	m_colorTextureID = NULL;
	m_depthTextureID = NULL;

	m_activatingEffect = -1;
	m_FBOCount = 0;
	m_numShader = 0;
	m_numEffect = 0;
	m_isLoaded = false;
}
///////////////////////////////////////////////////
EffectManager::~EffectManager()
{
	SAFE_DEL_ARRAY(m_shaders);
	SAFE_DEL_ARRAY(m_effects);
	SAFE_DEL_ARRAY(m_FBOId);
	SAFE_DEL_ARRAY(m_colorTextureID);
	SAFE_DEL_ARRAY(m_depthTextureID);
}
///////////////////////////////////////////////////
Shader* EffectManager::GetShaderByID(int id)
{
	for (int i = 0; i < m_numShader; i++)
		if (m_shaders[i].GetID() == id)
		{
			return m_shaders + i;
		}
	return NULL;
}
///////////////////////////////////////////////////
void EffectManager::Log()
{
}
///////////////////////////////////////////////////
void EffectManager::SetFBOCount(int count)
{
	m_FBOCount			= count;
	m_FBOId				= new GLuint[m_FBOCount];
	m_colorTextureID	= new GLuint[m_FBOCount];
	m_depthTextureID	= new GLuint[m_FBOCount];

	glGenFramebuffers(m_FBOCount, m_FBOId);
	for (int i = 0; i < m_FBOCount; i++)
	{
		glGenFramebuffers(1, &m_FBOId[i]);
		LOGI("AAAA: after  gen %d: %d", i, m_FBOId[i]);
		
		// generate color texture
		glGenTextures(1, &m_colorTextureID[i]);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_screenWidth, g_screenHeight, 0,
									   GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to GL_COLOR_ATTACHMENT0
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureID[i], 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Generate depth texture
		glGenTextures(1, &m_depthTextureID[i]);
		glBindTexture(GL_TEXTURE_2D, m_depthTextureID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, g_screenWidth, g_screenHeight, 0,
									   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to GL_DEPTH_ATTACHMENT
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTextureID[i], 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// Setup the Quad
	GLfloat quadVertices[] = {
		-0.9f, -0.9f, 0.0f,
		 0.9f, -0.9f, 0.0f,
		-0.9f,  0.9f, 0.0f,
		-0.9f,  0.9f, 0.0f,
		 0.9f, -0.9f, 0.0f,
		 0.9f,  0.9f, 0.0f,
		/*-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,*/
	};
	glGenBuffers(1, &m_quadVBOId);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
///////////////////////////////////////////////////
void EffectManager::Render()
{
	(m_effects + m_activatingEffect)->Render();
}
///////////////////////////////////////////////////
GLuint EffectManager::GetFBOIdAt(int index)
{
	if (index < m_FBOCount)
		return m_FBOId[index];
	return NULL;
}
///////////////////////////////////////////////////
void EffectManager::ActiveNextEffect()
{
	m_activatingEffect = (++m_activatingEffect) % m_numEffect;
	LOGI("Active Effect: %d, %s", m_activatingEffect, m_effects[m_activatingEffect].GetName());
}
///////////////////////////////////////////////////
bool EffectManager::ReadEMFile(const char* emDir)
{
	FILE*	pFile;
	char	buff[LINE_LENGTH], myString[LINE_LENGTH];	
	int		myInteger;
	float	x, y, z;
	pFile = fopen (emDir, "r" );
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}
	//-----------------
	READ_LINE();	// Shaders: 4
	sscanf(buff, "#Shaders: %u", &m_numShader);
	m_shaders	= new Shader[m_numShader];
	for (int i = 0; i < m_numShader; i++)
	{
		READ_LINE();	// ID
		sscanf(buff, "ID %u", &myInteger);
		m_shaders[i].SetID(myInteger);

		char strVSFile[LINE_LENGTH];
		char strFSFile[LINE_LENGTH];

		READ_LINE();	// VS
		strcpy(strVSFile, strtok(buff + 4, "\""));

		READ_LINE();	// FS
		strcpy(strFSFile, strtok(buff + 4, "\""));

		m_shaders[i].SetResult(m_shaders[i].Load(strVSFile, strFSFile));
	}
	READ_LINE();

	READ_LINE();	// #FBO: 3
	SetFBOCount(atoi(buff + strlen("#FBO: ")));
	READ_LINE();

	READ_LINE();	// #Effects: 2
	m_numEffect = atoi(strchr(buff, ':') + 1);
	m_effects = new Effect[m_numEffect];
	m_activatingEffect = 0;
	for (int i = 0; i < m_numEffect; i++)
	{
		READ_LINE();
		READ_LINE();	// ID
		m_effects[i].SetID(atoi(buff + strlen("ID ")));

		READ_LINE();	// Name
		sscanf(buff, "Name: %s", myString);
		m_effects[i].SetName(myString);

		READ_LINE();	// NumPasses
		m_effects[i].SetPassCount(atoi(strchr(buff, ':') + 1));
		for (int k = 0; k < m_effects[i].GetPassCount(); k++)
		{
			READ_LINE();	// Pass ID, name
			READ_LINE();	// ShaderID:
			Pass* pass = m_effects[i].GetPassAt(k);
			pass->SetShader(GetShaderByID(atoi(strchr(buff, ':') + 1)));

			READ_LINE();	// Textures
			char sTexture[3][3];
			memset(sTexture, 0, 3 * 3);
			strncpy(sTexture[0], strchr(buff, ':') + 2, 2);
			strncpy(sTexture[1], strchr(buff, ':') + 6, 2);
			strncpy(sTexture[2], strchr(buff, ':') + 10, 2);
			//LOGI("AAAA: %s ++ %s ++ %s", sTexture[0], sTexture[1], sTexture[2]);
			for (int l = 0; l < 3; l++)
			{
				GLint textID;
				if (strstr(sTexture[l], "1c") != NULL)
					textID = m_colorTextureID[0];
				else if (strstr(sTexture[l], "2c") != NULL)
					textID = m_colorTextureID[1];
				else if (strstr(sTexture[l], "3c") != NULL)
					textID = m_colorTextureID[2];
				else if (strstr(sTexture[l], "1d") != NULL)
					textID = m_depthTextureID[0];
				else
					textID = -1;
				pass->SetTexture(textID, l);
			}

			READ_LINE();	// Target
			pass->SetTarget(atoi(strchr(buff, ':') + 1));

			READ_LINE();	// OtherData
			pass->SetOtherData(atof(strchr(buff, ':') + 1));
		}
	}
	//-----------------
	fclose (pFile);
	m_isLoaded = true;
	return true;
}
///////////////////////////////////////////////////
