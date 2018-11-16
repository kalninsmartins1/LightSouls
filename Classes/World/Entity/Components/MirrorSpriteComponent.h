#pragma once

#include "LightSoulsTypes.h"

class Entity;

class MirrorSpriteComponent: public cocos2d::Component
{
public:
	CREATE_FUNC(MirrorSpriteComponent);

	void SetOwnerEntity(Entity* ownerEntity);	
	virtual void update(float delta) override;

private:
	Entity* m_ownerEntity;
};


