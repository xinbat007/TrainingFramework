#pragma once

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class TouchManager
{
protected:
	static TouchManager*	ms_pInstance;
private:
	GLint					xLBefore, yLBefore;
	GLint					xRBefore, yRBefore;
public:
	void					OnTouchPress(int id, int x, int y);
	void					OnTouchRelease(int id, int x, int y);
	void					OnTouchMove(int id, int x, int y);
	static TouchManager*	GetInstance()
	{
		if (ms_pInstance == NULL)
		{
			printf("CREATE TouchManager\n");
			ms_pInstance = new TouchManager();
		}
		return ms_pInstance;
	};
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			printf("DESTROY TouchManager\n");
			delete(ms_pInstance);
			ms_pInstance = NULL;
		}
	};
};