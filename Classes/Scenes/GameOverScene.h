#pragma once

#include "cocos2d.h"


class GameOverScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* CreateScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

    virtual bool init() override;

private:
	void OnTryAgainPressed();
	void OnGoToMenuPressed();
};
