#pragma once

#include "Commons.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
protected:
	static ResourceManager* ms_pInstance;
private:
	int			m_numModel, m_numTexture, m_numCubeTexture, m_numShader;
	Model*		m_Models;
	Texture*	m_Textures;
	Texture*	m_CubeTextures;
	Shader*		m_Shaders;

public:
	bool		ReadRMFile(const char* rmDir);
	Model*		GetModelById(int id);
	Texture*	GetTextureById(int id);
	Texture*	GetCubeTextureById(int id);
	Shader*		GetShaderById(int id);
	void		ListModel();
	void		ListTexture();
	void		ListCubeTexture();
	void		ListShader();
	~ResourceManager();
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
		{
			printf("CREATE ResourceManager\n");
			ms_pInstance = new ResourceManager();
		}
	};
	static ResourceManager* GetInstance()
	{
		return ms_pInstance;
	};
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			printf("DESTROY ResourceManager\n");
			delete(ms_pInstance);
			ms_pInstance = NULL;
		}
	}
};