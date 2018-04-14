#include "World.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Physics/PhysicsBodyConfig.h"
#include "Physics/PhysicsManager.h"
#include "3rdParty/Physics/PhysicsShapeCache.h"

World* World::Create(const std::string& pathToXML)
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

bool World::Init(const std::string& pathToXML)
{
	return XMLLoader::LoadWorld(*this, pathToXML);
}

bool World::Init(const std::string& pathToSprite, const std::string& bodyName, const std::string& pathToCollisionData)
{
	bool isSuccessful = initWithFile(pathToSprite);;
	
	PhysicsShapeCache* physicsShapeCache = PhysicsShapeCache::getInstance();
	isSuccessful = isSuccessful && physicsShapeCache->addShapesWithFile(pathToCollisionData);
	isSuccessful = isSuccessful && physicsShapeCache->setBodyOnSprite(bodyName, this);

	// If all operations were successful then init was successful
	return isSuccessful;
}