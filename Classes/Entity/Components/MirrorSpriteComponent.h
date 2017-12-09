#pragma once

#include "cocos2d.h"

class Player;

class MirrorSpriteComponent: public cocos2d::Component
{
public:
	CREATE_FUNC(MirrorSpriteComponent);
	virtual void setOwner(cocos2d::Node* owner) override;
	virtual void update(float delta) override;

private:
	Player* m_pPlayer;
};

