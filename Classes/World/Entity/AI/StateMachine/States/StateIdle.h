
#include "AState.h"

NS_LIGHTSOULS_BEGIN

class StateIdle : public AState
{
public:
	StateIdle(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;
	
	virtual void			OnEnter(AnimComponent * animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String & receivedEvent, const AEventData & eventData) override;

private:	
	EStateProgress	m_curProgress;
};

NS_LIGHTSOULS_END