#include "IState.h"

class StateAttack: public IState
{
public:
	StateAttack();

	virtual void onEnter() override;
	virtual StateProgress onStep() override;
	virtual void onExit() override;
	AIState getStateType() override;

private:
	StateProgress m_curProgress;
};
