#pragma once

#include "Commons.h"
#include "Effect.h"

class EffectManager
{
protected:
	static EffectManager* ms_pInstance;
private:
public:
	EffectManager();
	~EffectManager();
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
		{
			printf("CREATE EffectManager\n");
			ms_pInstance = new EffectManager;
		}
	};
	static EffectManager* GetInstance()
	{
		return ms_pInstance;
	};
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			printf("DESTROY EffectManager\n");
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	};

private:
	GLuint	m_quadVBOId;
	Shader*	m_shaders;
	Effect* m_effects;
	int		m_numShader;
	int		m_numEffect;

	GLuint*	m_FBOId;
	GLuint*	m_colorTextureID;
	GLuint*	m_depthTextureID;
	int		m_FBOCount;
	int		m_activatingEffect;
	bool	m_isLoaded;

public:
	bool	ReadEMFile(const char* emDir);
	void	SetFBOCount(int count);
	Shader*	GetShaderByID(int id);
	GLuint	GetFBOIdAt(int index);
	void	ActiveNextEffect();
	void	Render();
	void	Log();
	GLuint	GetQuadVBOId()		{ return m_quadVBOId; }
	int		GetEffectCount()	{ return m_numEffect; }
	bool	IsLoaded()			{ return m_isLoaded; }
};