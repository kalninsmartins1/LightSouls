#include "World.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Physics/PhysicsBodyConfig.h"
#include "Physics/PhysicsManager.h"

const String World::s_nodeName = "World";
bool World::s_isShapeCacheInitialized = false;

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

bool World::Init(const String& pathToSprite)
{
	bool isSuccessful = initWithFile(pathToSprite);;
	
	setName(s_nodeName);
	setLocalZOrder(GameConsts::WORLD_LAYER);
	cc::PhysicsBody* physicsBody = getPhysicsBody();
	isSuccessful &= physicsBody != nullptr;	
	if (isSuccessful) 
	{
		cc::Size size = getContentSize();
		physicsBody->setPositionOffset(Vector2(size.width * 0.5f, size.height * 0.5f));
	}

	// If all operations were successful then init was successful
	return isSuccessful;
}


