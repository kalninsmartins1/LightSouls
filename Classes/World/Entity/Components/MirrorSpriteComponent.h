#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity;

class MirrorSpriteComponent: public cocos2d::Component
{
public:
	CREATE_FUNC(MirrorSpriteComponent);

	void setOwnerEntity(Entity* ownerEntity);	
	virtual void update(float delta) override;

private:
	Entity* m_ownerEntity;
};

NS_LIGHTSOULS_END
