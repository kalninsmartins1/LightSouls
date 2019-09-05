#include "AState.h"

namespace tinyxml2
{
	class XMLElement;
};
using XMLElement = tinyxml2::XMLElement;

class StateIsPlayerClose : public AState
{
public:
	StateIsPlayerClose(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;

	virtual void			OnEnter(AnimComponent& animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;	
	virtual void			LoadXMLData(const XMLElement& xmlElement);

private:	
	AnimComponent*  m_animComponent;
	EStateProgress	m_curProgress;
	float			m_closeDistance;
};

