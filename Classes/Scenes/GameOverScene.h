#pragma once

#include "Classes/External/CocosEngine.h"


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
	void SwitchToGameScene();
};
