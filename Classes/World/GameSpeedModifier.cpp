#include "GameSpeedModifier.h"
#include "Utils/XML/XMLLoader.h"
#include "Scenes/GameScene.h"

GameSpeedModifier::GameSpeedModifier(GameScene& gameScene, cc::EventDispatcher& eventDispatcher)
	: m_gameScene(gameScene)
	, m_eventDispatcher(eventDispatcher)
	, m_isModificationActive(false)
{

}

GameSpeedModifier::~GameSpeedModifier()
{

}

bool GameSpeedModifier::Init(const String& pathToXML)
{
	return XMLLoader::InitializeGameSpeedModifier(*this, pathToXML);
}

void GameSpeedModifier::AddEvent(const String& eventName, float speed, float duration)
{
	EventParameters params;
	params.speed = speed;
	params.duration = duration;
	m_eventData[eventName] = params;

	m_eventDispatcher.addCustomEventListener(eventName,
		CC_CALLBACK_1(GameSpeedModifier::OnEventTriggered, this));
}

cc::PhysicsWorld* GameSpeedModifier::GetPhysicsWorld() const
{
	auto director = cc::Director::getInstance();
	cc::PhysicsWorld* world = nullptr;

	if (director != nullptr)
	{
		world = director->getRunningScene()->getPhysicsWorld();
	}

	return world;
}

float GameSpeedModifier::GetPhysicsWorldSpeed() const
{
	float speed = 1.0f;
	cc::PhysicsWorld* world = GetPhysicsWorld();
	if (world != nullptr)
	{
		speed = world->getSpeed();
	}

	return speed;
}

void GameSpeedModifier::SetPhysicsWorldSpeed(float speed) const
{
	cc::PhysicsWorld* world = GetPhysicsWorld();
	if (world != nullptr)
	{
		world->setSpeed(speed);
	}
}

void GameSpeedModifier::OnEventTriggered(cc::EventCustom* eventData)
{
	if (!m_isModificationActive && eventData != nullptr)
	{
		// Get event parameters
		const String& keyEvent = eventData->getEventName();
		const EventParameters& params = m_eventData[keyEvent];		
		m_originalTimeScale = m_gameScene.GetTimeModifier();		
		
		// Modify game speed by given parameters
 		const float& speed = params.speed;		
		m_gameScene.SetTimeModifier(speed);
 		SetPhysicsWorldSpeed(speed);

		// Run callback to disable the speed modification
		auto delay = cc::DelayTime::create(params.duration);
		auto callback = cc::CallFunc::create(CC_CALLBACK_0(GameSpeedModifier::OnSpeedModificationFinished, this));
		auto sequence = cc::Sequence::create(delay, callback, nullptr);
		auto director = cc::Director::getInstance();
		m_gameScene.runAction(sequence);
		m_isModificationActive = true;
	}
}

void GameSpeedModifier::OnSpeedModificationFinished()
{
	m_isModificationActive = false;
	m_gameScene.SetTimeModifier(m_originalTimeScale);
	SetPhysicsWorldSpeed(m_originalTimeScale);
}

