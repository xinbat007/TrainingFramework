#include <stdafx.h>
#include "StatesStack.h"
#include "Commons.h"
////////////////////////////////////////////////////////////////////////
StatesStack* StatesStack::ms_pInstance = NULL;
////////////////////////////////////////////////////////////////////////
StatesStack::StatesStack()
{
	m_stateIndex			= -1;
	m_stateCountToDelete	= 0;
}
////////////////////////////////////////////////////////////////////////
void StatesStack::Release()
{
}
////////////////////////////////////////////////////////////////////////
void StatesStack::PushState(StateBase* pState)
{
    m_pStatePushed.push(pState);
}
////////////////////////////////////////////////////////////////////////
void StatesStack::PopState(bool bResume)
{
	m_pStatePoped.push(m_pStateStack[m_stateIndex - m_pStatePoped.size()]);
}
////////////////////////////////////////////////////////////////////////
void StatesStack::MarkStateToDelete(StateBase* pState )
{
	m_pStateStackToDelete[m_stateCountToDelete] = pState;
	m_stateCountToDelete++;
}
////////////////////////////////////////////////////////////////////////
void StatesStack::DeleteStatesList()
{
	while (m_stateCountToDelete)
	{
		m_stateCountToDelete--;
		SAFE_DEL (m_pStateStackToDelete[m_stateCountToDelete]);
	}
}
////////////////////////////////////////////////////////////////////////
void StatesStack::ClearStateStack()
{
	while (m_stateIndex >= 0)
	{
		StateBase* pState = m_pStateStack[m_stateIndex];
		pState->Pause();
		pState->Release();
		MarkStateToDelete(pState);
		m_stateIndex--;
	}
	while(!m_pStatePushed.empty())
        m_pStatePushed.pop();
	while(!m_pStatePoped.empty())
        m_pStatePoped.pop();
}
////////////////////////////////////////////////////////////////////////
void StatesStack::Update(float deltaTime)
{
	while(!m_pStatePoped.empty())
	{
		StateBase* pState = m_pStatePoped.front();
        m_pStatePoped.pop();
		pState->Pause();
		pState->Release();
		m_stateIndex--;
		MarkStateToDelete(pState);
		if (m_stateIndex >= 0)
			m_pStateStack[m_stateIndex]->Resume();
		if (CurrentState())
		{
			//CurrentState()->ResetControls();
		}
	}
	while(!m_pStatePushed.empty())
	{
		StateBase* pState = m_pStatePushed.front();
        m_pStatePushed.pop();
		if (CurrentState())
		{
			//CurrentState()->ResetControls();
		}
		if (pState == NULL)
		{
			LOGI("Tried to add a NULL state !!!");
			ClearStateStack();
		}
		StateBase* oldState = CurrentState();
		if (oldState)
			oldState->Pause();
		m_stateIndex++;
		m_pStateStack[m_stateIndex] = pState;
		if (pState->Create() < 0)
		{		
			LOGI("Failed to create the current state!!!!\n");
			ClearStateStack();
		}
		else
		{
			pState->Resume();
		}
	}
	DeleteStatesList();
	m_pStateStack[m_stateIndex]->Update(deltaTime);
}
////////////////////////////////////////////////////////////////////////
void StatesStack::Key(unsigned char key, bool bIsPressed)
{
	if (CurrentState())
		CurrentState()->Key(key, bIsPressed);
}
////////////////////////////////////////////////////////////////////////
void StatesStack::Render()
{
	if (CurrentState())
		CurrentState()->Render();
}
////////////////////////////////////////////////////////////////////////
void StatesStack::Mouse(unsigned int mouseEvent, int x, int y, int id)
{
	if (CurrentState())
		CurrentState()->Mouse(mouseEvent, x, y, id);
}
////////////////////////////////////////////////////////////////////////