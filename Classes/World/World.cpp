#include "World.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Physics/PhysicsBodyConfig.h"
#include "Physics/PhysicsManager.h"
#include "3rdParty/Physics/PhysicsShapeCache.h"

NS_LIGHTSOULS_BEGIN

const String World::s_nodeName = "World";

const String& World::GetNodeName()
{
	return s_nodeName;
}

World* World::Create(const String& pathToXML)
{
	World* world = new (std::nothrow) World();
	if (world && world->Init(pathToXML))
	{
		world->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(world);
	}

	return world;
}

bool World::Init(const String& pathToXML)
{
	return XMLLoader::LoadWorld(*this, pathToXML);
}

bool World::Init(const String& pathToSprite, const String& bodyName, const String& pathToCollisionData)
{
	bool isSuccessful = initWithFile(pathToSprite);;
	
	PhysicsShapeCache* physicsShapeCache = PhysicsShapeCache::getInstance();
	isSuccessful = isSuccessful && physicsShapeCache->addShapesWithFile(pathToCollisionData);
	isSuccessful = isSuccessful && physicsShapeCache->setBodyOnSprite(bodyName, this);
	setName(s_nodeName);

	// If all operations were successful then init was successful
	return isSuccessful;
}

NS_LIGHTSOULS_END