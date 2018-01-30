#pragma once

#include "Pass.h"

class Effect
{
public:
	Effect(void);
	~Effect(void);

private:
	int		m_id;
	char*	m_strName;
	Pass*	m_pass;
	int		m_passCount;

public:
	void	SetPassCount(int count);
	void	SetName(const char* name);
	void	SetID(int id)			{ m_id = id; }

	int		GetID()					{ return m_id; }
	char*	GetName()				{ return m_strName; }
	int		GetPassCount()			{ return m_passCount; }
	Pass*	GetPassAt(int index)	{ return m_pass + index; }
	void	Render();
	void	Log();
};