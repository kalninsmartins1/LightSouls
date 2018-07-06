#pragma once

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene
{
public:
	MainMenuScene();

public:
    static cocos2d::Scene* CreateScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(MainMenuScene);

    virtual bool	init() override;

private:
	void OnStartPressed();
	void OnQuitPressed();
	void SwitchToGameScene();
};
