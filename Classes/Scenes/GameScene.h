#pragma once

#include "cocos2d.h"

namespace LightSouls
{
	class Player;
	class ProgressBar;
};

namespace cocos2d
{
	namespace ui
	{
		class Text;
	};
};

class GameScene : public cocos2d::Scene
{
public:
	GameScene();
	~GameScene();

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
	void OnAgentDestroyed(cocos2d::EventCustom* eventData);

private:
	LightSouls::Player*							m_player;
	LightSouls::ProgressBar*					m_healthBar;
	LightSouls::ProgressBar*					m_staminaBar;
	std::vector<cocos2d::EventListenerCustom*>	m_eventListeners;
	cocos2d::ui::Text*							m_scoreText;
};
