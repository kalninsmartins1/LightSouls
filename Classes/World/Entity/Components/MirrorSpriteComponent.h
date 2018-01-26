#pragma once

#include "cocos2d.h"

class Entity;

class MirrorSpriteComponent: public cocos2d::Component
{
public:
	CREATE_FUNC(MirrorSpriteComponent);

	void setOwnerEntity(Entity* owner);	
	virtual void update(float delta) override;

private:
	Entity* m_pEntity;
};

