#include "World.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Physics/PhysicsBodyConfig.h"
#include "Physics/PhysicsManager.h"

const String World::s_nodeName = "World";

World::World()
	: m_physicsBodyAnchor(Vector2::GetZero())
{	
	cc::EventDispatcher* eventDispatcher = getEventDispatcher();
	if (eventDispatcher != nullptr)
	{
		eventDispatcher->addCustomEventListener(PhysicsManager::GetEventOnPhysicsBodyAnchorSet().GetCStr(),
			CC_CALLBACK_1(World::OnPhysicsBodyAchorSetEvent, this));
	}
}

const String& World::GetNodeName()
{
	return s_nodeName;
}

World* World::Create(const String& pathToXML)
{
	World* world = new (std::nothrow) World();
	if (world && world->InitWithXML(pathToXML))
	{
		world->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(world);
	}

	return world;
}

bool World::InitWithXML(const String& pathToXML)
{
	return XMLLoader::LoadWorld(*this, pathToXML);
}

void World::OnPhysicsBodyAchorSetEvent(cc::EventCustom* eventData)
{
	if (eventData != nullptr)
	{
		Vector2* offset = static_cast<Vector2*>(eventData->getUserData());
		if (offset != nullptr)
		{
			m_physicsBodyAnchor = *offset;
		}
	}
}

bool World::Init(const String& pathToSprite)
{
	bool isSuccessful = initWithFile(pathToSprite.GetCStr());;	
	setName(s_nodeName.GetCStr());
	setLocalZOrder(GameConsts::WORLD_LAYER);

	cc::PhysicsBody* physicsBody = getPhysicsBody();
	isSuccessful &= physicsBody != nullptr;	

	if (isSuccessful) 
	{
		cc::Size size = getContentSize();
		physicsBody->setPositionOffset(cc::Vec2(size.width * m_physicsBodyAnchor.GetX(),
			size.height * m_physicsBodyAnchor.GetY()));
	}

	// If all operations were successful then init was successful
	return isSuccessful;
}


