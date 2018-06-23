#include "PhysicsManager.h"
#include "PhysicsBodyConfig.h"
#include "GameConsts.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN


const LightSouls::String LightSouls::PhysicsManager::s_onCollisionBeginEvent = "EVENT_ON_COLLISION_BEGIN";

PhysicsManager* PhysicsManager::GetInstance()
{
	static PhysicsManager instance;
	return &instance;
}

const String& LightSouls::PhysicsManager::GetEventOnCollisionBegin()
{
	return s_onCollisionBeginEvent;
}

bool PhysicsManager::Init(cocos2d::Node* context)
{	
	m_context = context;

	// Register for contact events
	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PhysicsManager::OnContactBegin, this);	
	contactListener->onContactSeparate = CC_CALLBACK_1(PhysicsManager::OnContactEnd, this);
	context->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, 
		context);

	m_debugDrawNode = cocos2d::DrawNode::create();
	m_debugDrawNode->setCameraMask(static_cast<unsigned int>(cocos2d::CameraFlag::USER1));
	m_context->addChild(m_debugDrawNode);

	return context != nullptr;
}

void PhysicsManager::RemoveContactBeginListener(const String& bodyName)
{
	for (unsigned int index = 0; index < m_beginContactListeners.size(); index++)
	{
		if (m_beginContactListeners[index].name == bodyName)
		{
			m_beginContactListeners.erase(m_beginContactListeners.begin() + index);
			break;
		}
	}	
}

void PhysicsManager::AddContactBeginListener(const String& bodyName,
	ContactCallback onContactBegin)
{
	m_beginContactListeners.emplace_back(PhysicsContactListener(bodyName, onContactBegin));
}

void PhysicsManager::AddContactEndListener(const String& bodyName, ContactCallback onContactEnd)
{
	m_endContactListeners.emplace_back(PhysicsContactListener(bodyName, onContactEnd));
}

void PhysicsManager::AddPhysicsBody(cocos2d::Node& attachmentNode,
	const PhysicsBodyConfig& bodyConfig)
{
	using namespace cocos2d;
	PhysicsBody* physicsBody = PhysicsBody::createBox(bodyConfig.GetSize(),
		bodyConfig.GetPhysicsMaterial());
	physicsBody->setDynamic(bodyConfig.IsBodyDynamic());
	physicsBody->setGravityEnable(bodyConfig.IsGravityEnabled());

	physicsBody->setCollisionBitmask(bodyConfig.GetCollisionBitMask());
	physicsBody->setContactTestBitmask(bodyConfig.GetCollisionBitMask());
	physicsBody->setCategoryBitmask(bodyConfig.GetCollisionBitMask());

	physicsBody->setName(RIGID_BODY_COMPONENT);
	physicsBody->setRotationEnable(bodyConfig.IsRotationEnabled());
	attachmentNode.addComponent(physicsBody);
}

void PhysicsManager::DebugDrawRect(const cocos2d::Rect& rect)
{
	const Vector2 pointOne(rect.getMinX(), rect.getMinY());
	const Vector2 pointTwo(rect.getMaxX(), rect.getMinY());
	const Vector2 pointThree(rect.getMaxX(), rect.getMaxY());
	const Vector2 pointFour(rect.getMinX(), rect.getMaxY());
	
	m_debugDrawNode->clear();
	m_debugDrawNode->drawRect(pointOne, pointTwo, pointThree, pointFour,
		cocos2d::Color4F::GREEN);
}

void PhysicsManager::QuerryRect(const cocos2d::Rect& rect, 
	const QueryRectCallback& callback)
{	
	cocos2d::PhysicsWorld* pWorld = cocos2d::Director::getInstance()->
		getRunningScene()->getPhysicsWorld();
	pWorld->queryRect(callback, rect, nullptr);
}

bool PhysicsManager::DispatchContactEventsToListeners(const cocos2d::PhysicsBody* bodyA, const cocos2d::PhysicsBody* bodyB, const std::vector<PhysicsContactListener>& listeners)
{
	cocos2d::Node* bodyANode = bodyA->getNode();
	cocos2d::Node* bodyBNode = bodyB->getNode();

	const String& bodyAName = bodyANode->getName();
	const String& bodyBName = bodyBNode->getName();

	// If contact contains some of the listener names then inform the listeners
	bool shouldCollide = true;
	for (auto& listener : listeners)
	{
		if (bodyAName == listener.name)
		{
			shouldCollide = shouldCollide && listener.onContactCallback(bodyB);
		}
		else if (bodyBName == listener.name)
		{
			shouldCollide = shouldCollide && listener.onContactCallback(bodyA);
		}
	}

	return shouldCollide;
}

bool PhysicsManager::OnContactBegin(cocos2d::PhysicsContact& contact)
{
	const cocos2d::PhysicsBody* bodyA = contact.getShapeA()->getBody();
	const cocos2d::PhysicsBody* bodyB = contact.getShapeB()->getBody();		

	bool shouldCollide = true;
	if (bodyA != nullptr && bodyB != nullptr)
	{
		shouldCollide = DispatchContactEventsToListeners(bodyA, bodyB, m_beginContactListeners);
	}

	return shouldCollide;
}

bool PhysicsManager::OnContactEnd(cocos2d::PhysicsContact& contact)
{
	const cocos2d::PhysicsBody* bodyA = contact.getShapeA()->getBody();
	const cocos2d::PhysicsBody* bodyB = contact.getShapeB()->getBody();

	if (bodyA != nullptr && bodyB != nullptr)
	{
		DispatchContactEventsToListeners(bodyA, bodyB, m_endContactListeners);
	}

	return true;
}

NS_LIGHTSOULS_END

