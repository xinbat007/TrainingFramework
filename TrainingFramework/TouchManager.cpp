#include <stdafx.h>
#include "TouchManager.h"
#include "Commons.h"
#include "Camera.h"
#include "SceneManager.h"
////////////////////////////////////////////////////////////////////////
TouchManager* TouchManager::ms_pInstance = NULL;
////////////////////////////////////////////////////////////////////////
void TouchManager::OnTouchPress(int id, int x, int y)
{
	if (id == 0)	// Left Mouse
	{
		LOGI("AAAA: OnTouchPress: %d, %d", x, y);
		xLBefore = x;
		yLBefore = y;
	}
	else if (id == 1)	// Right Mouse
	{
		xRBefore = x;
		yRBefore = y;
	}
}
////////////////////////////////////////////////////////////////////////
void TouchManager::OnTouchRelease(int id, int x, int y)
{
	if (id == 0)	// Left Mouse
	{
	}
	else if (id == 1)	// Right Mouse
	{
	}
}
////////////////////////////////////////////////////////////////////////
void TouchManager::OnTouchMove(int id, int x, int y)
{
	if (id == 0)	// Left Mouse
	{
		float dX = (x - xLBefore) * PI / 200.0f;
		float dY = (y - yLBefore) * PI / 200.0f;
		//SceneManager::GetInstance()->GetActiveCamera()->SetRotationY(dX);
		//SceneManager::GetInstance()->GetActiveCamera()->SetRotationX(dY);
		xLBefore = x;
		yLBefore = y;
	}
	else if (id == 1)	// Right Mouse
	{
		float dX = (x - xRBefore);// / 2.0f;
		float dY = (y - yRBefore);// / 2.0f;
		xRBefore = x;
		yRBefore = y;
		//SceneManager::GetInstance()->GetActiveCamera()->SetTranslateYZ(dX, -dY);
	}
}