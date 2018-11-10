#pragma once

#include "cocos2d.h"

enum class ENextScene;

class LoadingScreenScene : public cocos2d::Scene
{
public:
	LoadingScreenScene(const ENextScene& nextScene);

public:
    static cocos2d::Scene* CreateScene(const ENextScene& nextScene);	

    virtual bool	init() override;	

private:
	void SwitchToNextScene();

private:
	ENextScene	m_nextScene;	
};
