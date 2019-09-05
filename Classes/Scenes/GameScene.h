#pragma once

#include "Classes/External/CocosEngine.h"
#include "World/GameSpeedModifier.h"
#include "World/VFX/VFXManager.h"

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

	static cocos2d::Scene* CreateScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

    virtual bool init() override;
	virtual void update(float deltaTime) override;

private:
	Node* InitWolrdLayer(); // Returns world layer
	void InitUILayer();
	void StartGameOverFadeIn(float time);
	void SwitchToGameOverScene();
	void DisableMouseCursor();
	void ProcessDebugPhysicsDraw();
	void OnPlayerHealthChanged(cocos2d::EventCustom* eventData);
	void OnPlayerStaminaChanged(cocos2d::EventCustom* eventData);
	void OnPlayerDisappeared();
	void OnAgentDestroyed(cocos2d::EventCustom* eventData);
	void ReloadGame();
	void RegisterForEvents();
	void InitVFXManger(cocos2d::Node* worldLayer);

private:
	static PhysicsManager*			s_physicsManager;
	static GameInput*				s_gameInput;

	VFXManager						m_vfxManager;
	GameSpeedModifier				m_gameSpeedModifier;
	Player*							m_player;
	ProgressBar*					m_healthBar;
	ProgressBar*					m_staminaBar;
	cocos2d::ui::Text*				m_scoreText;
	bool							m_physicsDebugEnabled;
	float							m_timeModifier;
};
