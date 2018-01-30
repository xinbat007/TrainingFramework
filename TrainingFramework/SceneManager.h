#pragma once

#include "Commons.h"
#include "Object.h"
#include "Camera.h"

class SceneManager
{
protected:
	static SceneManager* ms_pInstance;
private:
	int			m_numObject, m_numCamera;
	Object*		m_Objects;
	Camera*		m_Cameras;
	int			m_activeCamera;
	float		m_fogStart, m_fogLength;
	bool		m_isRenderNormal;
	unsigned int m_MoveMask;
	unsigned int m_RotateMask;
public:
	bool		ReadSMFile(const char* smDir);
	void		ListObject();
	void		ListCamera();
	Object*		GetObjectByID(int id);
	Camera*		GetCameraByID(int id);
	void		Render();
	void		SetActiveCamera(int id) { m_activeCamera = id; }
	Camera*		GetActiveCamera() { return GetCameraByID(m_activeCamera); }
	float		GetFogStart() { return m_fogStart; }
	float		GetFogLength() { return m_fogLength; }
	void		Update(float deltaTime);
	void		Key(unsigned char key, bool bIsPressed);
	SceneManager();
	~SceneManager();
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
		{
			printf("CREATE SceneManager\n");
			ms_pInstance = new SceneManager();
		}
	};
	static SceneManager* GetInstance()
	{
		return ms_pInstance;
	};
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			printf("DESTROY SceneManager\n");
			delete(ms_pInstance);
			ms_pInstance = NULL;
		}
	};
};