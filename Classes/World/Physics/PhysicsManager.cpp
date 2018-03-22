#include "PhysicsManager.h"
#include "PhysicsBodyConfig.h"
#include "GameConsts.h"

PhysicsManager* PhysicsManager::GetInstance()
{
	static PhysicsManager instance;
	return &instance;
}

bool PhysicsManager::Init(cocos2d::Node* context)
{	
	m_context = context;

	// Register for contact events
	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PhysicsManager::OnContactBegin, this);
	context->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, 
		context);

	m_debugDrawNode = cocos2d::DrawNode::create();
	m_debugDrawNode->setCameraMask(static_cast<unsigned int>(cocos2d::CameraFlag::USER1));
	m_context->addChild(m_debugDrawNode);

	return context != nullptr;
}

void PhysicsManager::AddContactListener(const std::string& bodyName,
	ContactCallback onContactBegin)
{
	m_contactListeners.emplace_back(PhysicsContactListener(bodyName, onContactBegin));
}

void PhysicsManager::AddPhysicsBody(cocos2d::Node& attachmentNode,
	const PhysicsBodyConfig& bodyConfig)
{
	PhysicsBody* physicsBody = PhysicsBody::createBox(bodyConfig.GetSize(),
		bodyConfig.GetPhysicsMaterial());
	physicsBody->setDynamic(bodyConfig.IsBodyDynamic());
	physicsBody->setGravityEnable(bodyConfig.IsGravityEnabled());
	physicsBody->setContactTestBitmask(bodyConfig.GetCollisionBitMask());
	physicsBody->setCategoryBitmask(bodyConfig.GetCollisionBitMask());
	physicsBody->setName(RIGID_BODY_COMPONENT);
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
	const cocos2d::PhysicsQueryRectCallbackFunc& callback)
{	
	cocos2d::PhysicsWorld* pWorld = cocos2d::Director::getInstance()->
		getRunningScene()->getPhysicsWorld();
	pWorld->queryRect(callback, rect, nullptr);
}

bool PhysicsManager::OnContactBegin(cocos2d::PhysicsContact& contact)
{
	const PhysicsBody* bodyA = contact.getShapeA()->getBody();
	const PhysicsBody* bodyB = contact.getShapeB()->getBody();

	if (bodyA != nullptr && bodyB != nullptr)
	{
		const std::string& bodyAName = bodyA->getNode()->getName();
		const std::string& bodyBName = bodyB->getNode()->getName();

		// If contact contains some of the listener names then inform the listeners
		for(auto& listener : m_contactListeners)
		{
			if (bodyAName == listener.name)
			{
				listener.onContactCallback(bodyB);
			}
			else if (bodyBName == listener.name)
			{
				listener.onContactCallback(bodyA);
			}
		}
	}

	return true;
}
