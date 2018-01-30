#include <stdafx.h>
#include "Commons.h"
#include "EffectManager.h"
#include "GS_Welcome.h"
#include "ResourceManager.h"
#include "SceneManager.h"
////////////////////////////////////////////////////////////////////////
bool GS_Welcome::Create()
{
	LOGI("AAAA: Create Welcome");
	//--------------------------------
	SceneManager::GetInstance()->SetActiveCamera(2);
	//--------------------------------
	return true;
}
////////////////////////////////////////////////////////////////////////
bool GS_Welcome::Release()
{
	LOGI("AAAA: Release Welcome");
	return true;
}
////////////////////////////////////////////////////////////////////////
void GS_Welcome::Pause()
{
}
////////////////////////////////////////////////////////////////////////
void GS_Welcome::Resume()
{
}
////////////////////////////////////////////////////////////////////////
void GS_Welcome::Update(float deltaTime)
{
	SceneManager::GetInstance()->Update(deltaTime);
}
////////////////////////////////////////////////////////////////////////
void GS_Welcome::Render()
{
	if (EffectManager::GetInstance()->IsLoaded())
		glBindFramebuffer(GL_FRAMEBUFFER, EffectManager::GetInstance()->GetFBOIdAt(0));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SceneManager::GetInstance()->Render();
	if (EffectManager::GetInstance()->IsLoaded())
		EffectManager::GetInstance()->Render();
}
////////////////////////////////////////////////////////////////////////
void GS_Welcome::Key(unsigned char key, bool bIsPressed)
{
	SceneManager::GetInstance()->Key(key, bIsPressed);
	if (!bIsPressed)
	{
		switch (key)
		{
		case 4:
		//case '\x1b':	// 27, ESC
			{
			}
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////
void GS_Welcome::Mouse(int mouseEvent, int x, int y, int id)
{
	extern void MouseFuncDefault(int mouseEvent, int x, int y, int id);
	MouseFuncDefault(mouseEvent, x, y, id);
}