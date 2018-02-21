#include "PhysicsManager.h"
#include "Particle3D/PU/CCPUColorAffector.h"

PhysicsManager* PhysicsManager::getInstance()
{
	static PhysicsManager instance;
	return &instance;
}

bool PhysicsManager::init(cocos2d::Node* pContext)
{	
	m_pContext = pContext;

	// Register for contact events
	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PhysicsManager::onContactBegin, this);
	pContext->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, 
		pContext);

	m_pDebugDrawNode = cocos2d::DrawNode::create();
	m_pDebugDrawNode->setCameraMask((unsigned int)cocos2d::CameraFlag::USER1);
	m_pContext->addChild(m_pDebugDrawNode);

	return pContext != nullptr;
}

void PhysicsManager::addContactListener(const std::string& bodyName,
	ContactCallback onContactBegin)
{
	m_contactListeners.emplace_back(PhysicsContactListener(bodyName, onContactBegin));
}

void PhysicsManager::debugDrawRect(const cocos2d::Rect& rect)
{
	const Vector2 pointOne(rect.getMinX(), rect.getMinY());
	const Vector2 pointTwo(rect.getMaxX(), rect.getMinY());
	const Vector2 pointThree(rect.getMaxX(), rect.getMaxY());
	const Vector2 pointFour(rect.getMinX(), rect.getMaxY());
	
	m_pDebugDrawNode->clear();
	m_pDebugDrawNode->drawRect(pointOne, pointTwo, pointThree, pointFour,
		cocos2d::Color4F::GREEN);
}

void PhysicsManager::querryRect(const cocos2d::Rect& rect, 
	const cocos2d::PhysicsQueryRectCallbackFunc& callback)
{	
	cocos2d::PhysicsWorld* pWorld = cocos2d::Director::getInstance()->
		getRunningScene()->getPhysicsWorld();
	pWorld->queryRect(callback, rect, nullptr);
}

bool PhysicsManager::onContactBegin(cocos2d::PhysicsContact& contact)
{
	const PhysicsBody* pBodyA = contact.getShapeA()->getBody();
	const PhysicsBody* pBodyB = contact.getShapeB()->getBody();

	if (pBodyA != nullptr && pBodyB != nullptr)
	{
		const std::string& bodyAName = pBodyA->getNode()->getName();
		const std::string& bodyBName = pBodyB->getNode()->getName();

		// If contact contains some of the listener names then inform the listeners
		for(auto& listener : m_contactListeners)
		{
			if (bodyAName == listener.name)
			{
				listener.onContactCallback(pBodyB);
			}
			else if (bodyBName == listener.name)
			{
				listener.onContactCallback(pBodyA);
			}
		}
	}

	return true;
}
