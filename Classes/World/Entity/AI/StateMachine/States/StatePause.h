#include "AState.h"

NS_LIGHTSOULS_BEGIN

class StatePause : public AState
{
public:
	StatePause(AIAgent& aiAgent);

public:
	virtual AIState			GetStateType() const override;

	void					SetPauseTime(float time);

	virtual void			OnEnter(AnimComponent * animComponent) override;
	virtual StateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String & receivedEvent, const AEventData & eventData) override;

private:
	void OnPauseExpired();

private:
	AIAgent&		m_agent;	
	StateProgress	m_curProgress;
	float			m_pauseTime;
};

NS_LIGHTSOULS_END