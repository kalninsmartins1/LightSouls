#pragma once

#include "cocos2d.h"

class Player;

class MirrorSpriteComponent: public cocos2d::Component
{
public:
	CREATE_FUNC(MirrorSpriteComponent);

	void setPlayer(Player* owner);	
	virtual void update(float delta) override;

private:
	Player* m_pPlayer;
};

