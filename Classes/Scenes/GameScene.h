#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

namespace LightSouls
{
	class Player;
	class ProgressBar;
};

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* CreateScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

    virtual bool init() override;    
	virtual void update(float deltaTime) override;

private:
	void InitWolrdLayer();
	void InitUILayer();
	void OnPlayerHealthChanged(cocos2d::EventCustom* eventData);
	void OnPlayerStaminaChanged(cocos2d::EventCustom* eventData);

private:
	LightSouls::Player*			m_player;
	LightSouls::ProgressBar*	m_healthBar;
	LightSouls::ProgressBar*	m_staminaBar;
};

#endif // __HELLOWORLD_SCENE_H__
