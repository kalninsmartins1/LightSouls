#include "AState.h"



class Entity;
class AIAvoidTargetAction;

class StatePause : public AState
{
public:
	StatePause(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;	

	virtual void			OnEnter(AnimComponent& animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;	
	void					LoadXMLData(const XMLElement& xmlElement) override;

private:	
	bool					IsTargetTooFar() const;

	void OnPauseExpired();

private:
	const Entity*		 m_targetEntity;
	AIAvoidTargetAction* m_avoidAction;
	EStateProgress		 m_curProgress;

	float			m_pauseTime;
	float			m_maxDistaneceToTarget;
	bool			m_shouldCheckDistanceToTarget;
};

