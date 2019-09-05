#include "PhysicsManager.h"
#include "PhysicsBodyConfig.h"
#include "GameConsts.h"
#include "World/Entity/Entity.h"

const String PhysicsManager::s_onCollisionBeginEvent = "EVENT_ON_COLLISION_BEGIN";
const String PhysicsManager::s_onPhysicsBodyAnchorSet = "EVENT_ON_PHYSICS_BODY_OFFSET_SET";

const String& PhysicsManager::GetEventOnCollisionBegin()
{
	return s_onCollisionBeginEvent;
}

const String& PhysicsManager::GetEventOnPhysicsBodyAnchorSet()
{
	return s_onPhysicsBodyAnchorSet;
}

PhysicsManager* PhysicsManager::Create(cc::Node* context)
{
	PhysicsManager* manager = new (std::nothrow) PhysicsManager();
	if (manager == nullptr || !manager->Init(context))
	{
		CC_SAFE_DELETE(manager);
	}	

	return manager;
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

	InitDebugDraw();
	
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
	PhysicsBody* physicsBody = nullptr;
	switch (bodyConfig.GetBodyType())
	{				
		case BodyType::BOX:
			physicsBody = PhysicsBody::createBox(bodyConfig.GetSize(),
				bodyConfig.GetPhysicsMaterial());
			break;

		case BodyType::EDGE_BOX:
			physicsBody = PhysicsBody::createEdgeBox(bodyConfig.GetSize(),
				bodyConfig.GetPhysicsMaterial());
			break;
	}

	physicsBody->setDynamic(bodyConfig.IsBodyDynamic());
	physicsBody->setGravityEnable(bodyConfig.IsGravityEnabled());

	physicsBody->setCollisionBitmask(bodyConfig.GetCollisionBitMask());
	physicsBody->setContactTestBitmask(bodyConfig.GetCollisionBitMask());
	physicsBody->setCategoryBitmask(bodyConfig.GetCollisionCategory());

	physicsBody->setName(GameConsts::RIGID_BODY_COMPONENT);
	physicsBody->setRotationEnable(bodyConfig.IsRotationEnabled());	
	attachmentNode.addComponent(physicsBody);
}

void PhysicsManager::OnReload()
{
	InitDebugDraw();
}

void PhysicsManager::DebugDrawRect(const cocos2d::Rect& rect)
{
	const cc::Vec2 pointOne(rect.getMinX(), rect.getMinY());
	const cc::Vec2 pointTwo(rect.getMaxX(), rect.getMinY());
	const cc::Vec2 pointThree(rect.getMaxX(), rect.getMaxY());
	const cc::Vec2 pointFour(rect.getMinX(), rect.getMaxY());
	
	m_debugDrawNode->clear();
	m_debugDrawNode->drawRect(pointOne, pointTwo, pointThree, pointFour,
		cocos2d::Color4F::RED);
}

void PhysicsManager::QuerryRect(const cocos2d::Rect& rect, 
	const QueryRectCallback& callback)
{	
	cocos2d::PhysicsWorld* world = cocos2d::Director::getInstance()->
		getRunningScene()->getPhysicsWorld();
	world->queryRect(callback, rect, nullptr);
}

void PhysicsManager::Raycast(RaycastCallback callback, const Vector2& startPoint, const Vector2& endPoint, void* data)
{
	using namespace cocos2d;
	Director* director = Director::getInstance();
	PhysicsWorld* world = director->getRunningScene()->getPhysicsWorld();
	cc::Vec2 ccStartPoint = cc::Vec2(startPoint.GetX(), startPoint.GetY());
	cc::Vec2 ccEndPoint = cc::Vec2(endPoint.GetX(), endPoint.GetY());
	world->rayCast(callback, ccStartPoint, ccEndPoint, data);	
}

bool PhysicsManager::DispatchContactEventsToListeners(const Vector2& contactPoint, const cocos2d::PhysicsBody* bodyA, const cocos2d::PhysicsBody* bodyB, const std::vector<PhysicsContactListener>& listeners)
{
	cocos2d::Node* bodyANode = bodyA->getNode();
	cocos2d::Node* bodyBNode = bodyB->getNode();

	bool shouldCollide = true;
	if (bodyANode != nullptr && bodyBNode != nullptr)
	{
		const String& bodyAName = bodyANode->getName();
		const String& bodyBName = bodyBNode->getName();

		// If contact contains some of the listener names then inform the listeners		
		for (auto& listener : listeners)
		{
			if (bodyAName == listener.name)
			{
				shouldCollide = shouldCollide && listener.onContactCallback(contactPoint, bodyB);
			}
			else if (bodyBName == listener.name)
			{
				shouldCollide = shouldCollide && listener.onContactCallback(contactPoint, bodyA);
			}
		}
	}
	else
	{
		shouldCollide = false;
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
		auto ccVec = contact.getContactData()->points[0];
		const Vector2 vec(ccVec.x, ccVec.y);
		shouldCollide = DispatchContactEventsToListeners(vec, bodyA, bodyB, m_beginContactListeners);
	}

	return shouldCollide;
}

bool PhysicsManager::OnContactEnd(cocos2d::PhysicsContact& contact)
{
	const cocos2d::PhysicsBody* bodyA = contact.getShapeA()->getBody();
	const cocos2d::PhysicsBody* bodyB = contact.getShapeB()->getBody();

	if (bodyA != nullptr && bodyB != nullptr)
	{
		auto ccVec = contact.getContactData()->points[0];
		const Vector2 vec(ccVec.x, ccVec.y);
		DispatchContactEventsToListeners(vec, bodyA, bodyB, m_endContactListeners);
	}

	return true;
}

void PhysicsManager::InitDebugDraw()
{
	m_debugDrawNode = cocos2d::DrawNode::create();
	m_debugDrawNode->setCameraMask(static_cast<unsigned int>(cocos2d::CameraFlag::USER1));
	m_debugDrawNode->setLocalZOrder(GameConsts::DEBUG_LAYER);
	m_context->addChild(m_debugDrawNode);
}


