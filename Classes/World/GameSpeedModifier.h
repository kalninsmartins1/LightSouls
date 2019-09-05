#pragma once

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/String/String.h"

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
	static const String& GetEventOnModificationStarted();
	static const String& GetEventOnModificationEnded();

	bool Init(const String& pathToXML);

	void AddEvent(const String& eventName, float speed, float duration);

private:
	cocos2d::PhysicsWorld*	GetPhysicsWorld() const;
	float					GetPhysicsWorldSpeed() const;

	void					SetPhysicsWorldSpeed(float speed) const;

	void OnEventTriggered(cocos2d::EventCustom* eventData);
	void OnSpeedModificationFinished();

private:
	static String						s_eventOnModificationStarted;
	static String						s_eventOnModificationEnded;
	GameScene&							m_gameScene;
	cocos2d::EventDispatcher&			m_eventDispatcher;
	std::map<String, EventParameters>	m_eventData;
	float								m_originalTimeScale;	
	bool								m_isModificationActive;
};