#pragma once

#include "LightSoulsTypes.h"
#include "World/GameSpeedModifier.h"

class Player;
class ProgressBar;
class PhysicsManager;
class GameInput;
class ValueChangedEventData;

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
	float				   GetTimeModifier() const;

	void				   SetTimeModifier(float modifier);

	static cc::Scene* CreateScene();
	
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
	void HandlePlayerHealthChangedEvent(const ValueChangedEventData& eventData);
	void OnPlayerHealthChanged(cc::EventCustom* eventData);
	void OnPlayerStaminaChanged(cc::EventCustom* eventData);
	void OnAgentDestroyed(cc::EventCustom* eventData);

private:
	static PhysicsManager*			s_physicsManager;
	static GameInput*				s_gameInput;
	GameSpeedModifier				m_gameSpeedModifier;
	Player*							m_player;
	ProgressBar*					m_healthBar;
	ProgressBar*					m_staminaBar;
	cocos2d::ui::Text*				m_scoreText;
	bool							m_physicsDebugEnabled;
	float							m_timeModifier;
};
