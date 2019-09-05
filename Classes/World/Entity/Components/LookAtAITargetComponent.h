#pragma once

#include "Classes/External/CocosEngine.h"

class Entity;

class LookAtAITargetComponent : public cocos2d::Component
{
public:
	CREATE_FUNC(LookAtAITargetComponent);

	void			SetOwnerEntity(cocos2d::Node& ownerEntity);
	virtual void	update(float delta) override;

private:
	Entity* m_ownerEntity;
};