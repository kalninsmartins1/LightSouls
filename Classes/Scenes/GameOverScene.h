#pragma once

#include "cocos2d.h"


class GameOverScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* CreateScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

    virtual bool init() override;    
	virtual void update(float deltaTime) override;
};
