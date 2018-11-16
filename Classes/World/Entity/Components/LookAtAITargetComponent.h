#pragma once

#include "LightSoulsTypes.h"

class Entity;

class LookAtAITargetComponent : public cocos2d::Component
{
public:
	CREATE_FUNC(LookAtAITargetComponent);

	void			SetOwnerEntity(Entity* ownerEntity);
	virtual void	update(float delta) override;

private:
	Entity* m_ownerEntity;
};