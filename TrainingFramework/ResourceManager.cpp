#include <stdafx.h>
#include "ResourceManager.h"
///////////////////////////////////////////////////
ResourceManager* ResourceManager::ms_pInstance = NULL;
///////////////////////////////////////////////////
ResourceManager::~ResourceManager()
{
	SAFE_DEL_ARRAY(m_Models);
	SAFE_DEL_ARRAY(m_Textures);
	SAFE_DEL_ARRAY(m_CubeTextures);
	SAFE_DEL_ARRAY(m_Shaders);
}
///////////////////////////////////////////////////
Model* ResourceManager::GetModelById(int id)
{
	for (int i = 0; i < m_numModel; i++)
	{
		if (m_Models[i].GetID() == id)
			return m_Models + i;
	}
	return NULL;
}
///////////////////////////////////////////////////
Texture* ResourceManager::GetTextureById(int id)
{
	for (int i = 0; i < m_numTexture; i++)
	{
		if (m_Textures[i].GetID() == id)
			return m_Textures + i;
	}
	return NULL;
}
///////////////////////////////////////////////////
Texture* ResourceManager::GetCubeTextureById(int id)
{
	for (int i = 0; i < m_numCubeTexture; i++)
	{
		if (m_CubeTextures[i].GetID() == id)
			return m_CubeTextures + i;
	}
	return NULL;
}
///////////////////////////////////////////////////
Shader* ResourceManager::GetShaderById(int id)
{
	for (int i = 0; i < m_numShader; i++)
	{
		if (m_Shaders[i].GetID() == id)
			return m_Shaders + i;
	}
	return NULL;
}
///////////////////////////////////////////////////
void ResourceManager::ListModel()
{
	for (int i = 0; i < m_numModel; i++)
	{
		m_Models[i].Log();
	}
}
///////////////////////////////////////////////////
void ResourceManager::ListTexture()
{
	for (int i = 0; i < m_numTexture; i++)
	{
		m_Textures[i].Log();
	}
}
///////////////////////////////////////////////////
void ResourceManager::ListCubeTexture()
{
	for (int i = 0; i < m_numCubeTexture; i++)
	{
		m_CubeTextures[i].Log();
	}
}
///////////////////////////////////////////////////
void ResourceManager::ListShader()
{
	for (int i = 0; i < m_numShader; i++)
	{
		m_Shaders[i].Log();
	}
}
///////////////////////////////////////////////////
bool ResourceManager::ReadRMFile(const char* rmDir)
{
	FILE*	pFile;
	char	buff[LINE_LENGTH], myString[LINE_LENGTH];	
	int		myInteger;
	pFile = fopen (rmDir, "r" );
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}
	//-----------------
	READ_LINE();	// Textures
	sscanf(buff, "#2D Textures: %u", &m_numTexture);
	m_Textures		= new Texture[m_numTexture];
	for (int i = 0; i < m_numTexture; i++)
	{
		m_Textures[i].SetIsCubeTexture(false);

		READ_LINE();
		sscanf(buff, "ID %u", &myInteger);
		m_Textures[i].SetID(myInteger);

		READ_LINE();
		m_Textures[i].LoadTextureFromFile(strtok(buff + 6, "\""));

		READ_LINE();
		sscanf(buff, "MINFILTER %s", myString);
		m_Textures[i].SetMinFilter(myString);

		READ_LINE();
		sscanf(buff, "MAGFILTER %s", myString);
		m_Textures[i].SetMagFilter(myString);

		READ_LINE();
		sscanf(buff, "TILING %s", myString);
		m_Textures[i].SetTiling(myString);
	}
	READ_LINE();	// blank line

	READ_LINE();	// CUBE Textures
	sscanf(buff, "#Cube Textures: %u", &m_numCubeTexture);
	m_CubeTextures		= new Texture[m_numCubeTexture];
	for (int i = 0; i < m_numCubeTexture; i++)
	{
		m_CubeTextures[i].SetIsCubeTexture(true);

		READ_LINE();
		sscanf(buff, "ID %u", &myInteger);
		m_CubeTextures[i].SetID(myInteger);

		READ_LINE();
		m_CubeTextures[i].LoadTextureFromFile(strtok(buff + 6, "\""));

		READ_LINE();
		sscanf(buff, "MINFILTER %s", myString);
		m_CubeTextures[i].SetMinFilter(myString);

		READ_LINE();
		sscanf(buff, "MAGFILTER %s", myString);
		m_CubeTextures[i].SetMagFilter(myString);

		READ_LINE();
		sscanf(buff, "TILING %s", myString);
		m_CubeTextures[i].SetTiling(myString);
	}
	READ_LINE();	// blank line

	READ_LINE();
	sscanf(buff, "#Shader: %u", &m_numShader);
	m_Shaders		= new Shader[m_numShader];
	for (int i = 0; i < m_numShader; i++)
	{
		READ_LINE();
		sscanf(buff, "ID %u", &myInteger);
		m_Shaders[i].SetID(myInteger);

		READ_LINE();
		strcpy(myString, strtok(buff + 4, "\""));

		READ_LINE();
		m_Shaders[i].SetResult(m_Shaders[i].Load(myString, strtok(buff + 4, "\"")));
		
		READ_LINE();
		READ_LINE();
	}
	READ_LINE();	// blank line

	READ_LINE();	// Models
	sscanf(buff, "#Models: %u", &m_numModel);
	m_Models	= new Model[m_numModel];
	for (int i = 0; i < m_numModel; i++)
	{
		READ_LINE();
		sscanf(buff, "ID %u", &myInteger);
		m_Models[i].SetID(myInteger);
		
		READ_LINE();
		strcpy(myString, strtok(buff + 6, "\""));

		READ_LINE();
		m_Models[i].LoadModelFromFile(myString, strtok(buff + 11, "\""));
	}
	//-----------------
	fclose (pFile);
}
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
