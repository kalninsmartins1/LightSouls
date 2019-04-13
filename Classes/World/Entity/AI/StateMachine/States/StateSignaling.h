#include "AState.h"



class StateSignaling : public AState
{
public:
	StateSignaling(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;

	virtual void			OnEnter(AnimComponent& animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;	
	virtual void			LoadXMLData(const XMLElement& xmlElement);
	virtual void			OnEventReceived(const String& receivedEvent, const BaseEventData& eventData) override;

private:
	void OnFinishedSignaling();

private:	
	AnimComponent*  m_animComponent;
	EStateProgress	m_curProgress;
	float			m_signalTime;
};

