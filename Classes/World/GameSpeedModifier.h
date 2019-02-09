#pragma once

#include "LightSoulsTypes.h"

class GameScene;

struct EventParameters
{
	float speed;
	float duration;
};

class GameSpeedModifier
{
public:
	GameSpeedModifier(GameScene& gameScene, cc::EventDispatcher& eventDispatcher);
	~GameSpeedModifier();

public:
	bool Init(const String& pathToXML);

	void AddEvent(const String& eventName, float speed, float duration);

private:
	cc::PhysicsWorld* GetPhysicsWorld() const;
	float			  GetPhysicsWorldSpeed() const;

	void			  SetPhysicsWorldSpeed(float speed) const;

	void OnEventTriggered(cc::EventCustom* eventData);
	void OnSpeedModificationFinished();

private:
	GameScene&							m_gameScene;
	cc::EventDispatcher&				m_eventDispatcher;	
	std::map<String, EventParameters>	m_eventData;
	float								m_originalTimeScale;	
	bool								m_isModificationActive;
};