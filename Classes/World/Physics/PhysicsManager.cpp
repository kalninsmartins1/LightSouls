#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::getInstance()
{
	static PhysicsManager instance;
	return &instance;
}

bool PhysicsManager::init(cocos2d::Node* pContext)
{	
	// Register for contact events
	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PhysicsManager::onContactBegin, this);
	pContext->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, 
		pContext);

	return pContext != nullptr;
}

void PhysicsManager::addContactListener(const std::string& bodyName,
	ContactCallback onContactBegin)
{
	m_contactListeners.emplace_back(PhysicsContactListener(bodyName, onContactBegin));
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
