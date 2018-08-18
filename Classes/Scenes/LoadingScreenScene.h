#pragma once

#include "cocos2d.h"

namespace LightSouls
{
	enum class ENextScene;
};

class LoadingScreenScene : public cocos2d::Scene
{
public:
	LoadingScreenScene(LightSouls::ENextScene nextScene);

public:
    static cocos2d::Scene* CreateScene(LightSouls::ENextScene nextScene);	

    virtual bool	init() override;	

private:
	void SwitchToNextScene();

private:
	LightSouls::ENextScene	m_nextScene;	
};
