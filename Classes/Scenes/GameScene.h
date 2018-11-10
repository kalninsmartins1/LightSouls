#pragma once

#include "cocos2d.h"

class Player;
class ProgressBar;
class PhysicsManager;
class GameInput;

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
	static PhysicsManager* GetPhysicsManager();
	static GameInput*	   GetGameInput();

	static cocos2d::Scene* CreateScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

    virtual bool init() override;    
	virtual void update(float deltaTime) override;

private:
	void InitWolrdLayer();
	void InitUILayer();
	void StartGameOverFadeIn(float time);
	void SwitchToGameOverScene();
	void ProcessDebugPhysicsDraw();
	void OnPlayerHealthChanged(cocos2d::EventCustom* eventData);
	void OnPlayerStaminaChanged(cocos2d::EventCustom* eventData);
	void OnAgentDestroyed(cocos2d::EventCustom* eventData);

private:
	static PhysicsManager*			s_physicsManager;
	static GameInput*				s_gameInput;
	Player*							m_player;
	ProgressBar*					m_healthBar;
	ProgressBar*					m_staminaBar;
	cocos2d::ui::Text*							m_scoreText;
	bool										m_physicsDebugEnabled;
};
