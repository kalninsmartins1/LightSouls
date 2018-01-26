#pragma once
#include "IState.h"


class StateChase: public IState
{
public:
	StateChase();

	virtual void onEnter() override;
	virtual StateProgress onStep() override;
	virtual void onExit() override;
	virtual AIState getStateType() override;

private: 
	StateProgress m_curProgress;
};
