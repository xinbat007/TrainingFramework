#ifndef __GS_WELCOME_H__
#define __GS_WELCOME_H__

#include "Commons.h"
#include "Model.h"

class GS_Welcome : public StateBase
{
private:
public:
	virtual bool Create();
	virtual bool Release();

	virtual void Update(float deltaTime);
	virtual void Render();

	virtual void Pause();
	virtual void Resume();

	virtual void Key(unsigned char key, bool bIsPressed);
	virtual void Mouse(int mouseEvent, int x, int y, int id);
};


#endif