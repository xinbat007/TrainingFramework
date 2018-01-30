#ifndef _STATES_STACK_H_
#define _STATES_STACK_H_

#include <queue>
#include "StateBase.h"

#define GAME_STATES_STACK_SIZE	12 

class StatesStack
{
protected:
	static StatesStack*		ms_pInstance;
private:
	StateBase*				m_pStateStack[GAME_STATES_STACK_SIZE];
	int						m_stateIndex;
	StateBase*				m_pStateStackToDelete[GAME_STATES_STACK_SIZE];
	int						m_stateCountToDelete;

	std::queue<StateBase*>	m_pStatePushed;
	std::queue<StateBase*>	m_pStatePoped;
public:
	StatesStack();
	void					InitStateStack()  { m_stateIndex = -1; }
	void					Release();
	void					Update(float deltaTime);
	//void					ChangeState(gxGameState* pState, bool destroyPrevious = true);
	//void					PopToState(int state);
	void					PushState(StateBase* pState);
	void					PopState(bool bResume = true);
	void					ClearStateStack();
	void					MarkStateToDelete(StateBase* pState);
	void					DeleteStatesList();
	StateBase*				CurrentState() { return (m_stateIndex >= 0) ? m_pStateStack[m_stateIndex] : NULL; }
	void					Key( unsigned char key, bool bIsPressed);
	void					Render();
	void					Mouse(unsigned int mouseEvent, int x, int y, int id);
	static StatesStack*		GetInstance()
	{
		if (ms_pInstance == NULL)
		{
			//LOGI("CREATE StatesStack\n");
			ms_pInstance = new StatesStack();
		}
		return ms_pInstance;
	};
	static void				DestroyInstance()
	{
		if (ms_pInstance)
		{
			//LOGI("DESTROY StatesStack\n");
			delete(ms_pInstance);
			ms_pInstance = NULL;
		}
	}
};
#define S_StateMachine		StatesStack::GetInstance()

#endif