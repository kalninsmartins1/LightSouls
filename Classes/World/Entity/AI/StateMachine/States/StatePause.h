#include "AState.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class StatePause : public AState
{
public:
	StatePause(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;	

	virtual void			OnEnter(AnimComponent * animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;	
	void					LoadXMLData(const XMLElement* xmlElement) override;

private:	

	void OnPauseExpired();

private:
	const Entity*	m_targetEntity;	
	EStateProgress	m_curProgress;
	float			m_pauseTime;	
};

NS_LIGHTSOULS_END