#ifndef __STATE_BASE_H__
#define __STATE_BASE_H__

enum GameState
{
	GS_WELCOME,
	GS_MAINMENU,
	GS_GAMEPLAY,
	GS_OPTION,
	GS_CREDIT,
};

class StateBase
{
public:
	virtual bool Create() = 0;
	virtual bool Release() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void Key(unsigned char key, bool bIsPressed) = 0;
	virtual void Mouse(int mouseEvent, int x, int y, int id) = 0;
};


#endif