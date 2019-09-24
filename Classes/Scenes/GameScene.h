#pragma once

#include "Classes/External/CocosEngine.h"
#include "World/GameSpeedModifier.h"
#include "World/VFX/VFXManager.h"

class Player;
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

class GameScene : public cc::Scene
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
	void RegisterSystems();
	void RegisterAttributeLoaders();
	Node* InitWolrdLayer(); // Returns world layer
	void InitUILayer();
	void StartGameOverFadeIn(float time);
	void SwitchToGameOverScene();
	void OnAgentDestroyed(cc::EventCustom* eventData);
	void ProcessDebugPhysicsDraw();
	void OnPlayerDisappeared();
	void ReloadGame();
	void RegisterForEvents();
	void InitVFXManger(cc::Node* worldLayer);

private:
	static PhysicsManager*			s_physicsManager;
	static GameInput*				s_gameInput;

	VFXManager						m_vfxManager;
	GameSpeedModifier				m_gameSpeedModifier;
	Player*							m_player;
	cc::ui::Text*					m_scoreText;
	bool							m_physicsDebugEnabled;
	float							m_timeModifier;
};
