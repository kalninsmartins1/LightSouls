#include "AState.h"

NS_LIGHTSOULS_BEGIN

class StateSignaling : public AState
{
public:
	StateSignaling(AIAgent& aiAgent);

public:
	virtual AIState			GetStateType() const override;
	
	void					SetSignalingTime(float time);

	virtual void			OnEnter(AnimComponent * animComponent) override;
	virtual StateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String & receivedEvent, const AEventData & eventData) override;

private:
	void OnFinishedSignaling();

private:
	AIAgent&		m_agent;
	AnimComponent*  m_animComponent;
	StateProgress	m_curProgress;
	float			m_signalTime;
};

NS_LIGHTSOULS_END