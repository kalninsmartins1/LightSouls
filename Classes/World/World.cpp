#include "World.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Physics/PhysicsBodyConfig.h"
#include "Physics/PhysicsManager.h"

World* World::Create(const std::string& pathToXML)
{
	World* pWorld = new (std::nothrow) World();
	if (pWorld && pWorld->Init(pathToXML))
	{
		pWorld->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pWorld);
	}

	return pWorld;
}

bool World::Init(const std::string& pathToXML)
{
	return XMLLoader::LoadWorld(*this, pathToXML);
}

bool World::Init(const std::string& pathToSprite, int tileCount)
{
	using namespace cocos2d;

	// Add tiles visually
	int tilePositionX = 0;
	int tileHeight = 0;
	for (int count = 0; count < tileCount; count++)
	{
		Sprite* tile = Sprite::create(pathToSprite);
		tile->setPosition(Vec2(tilePositionX, 0));

		const Size& tileSize = tile->getContentSize();
		tilePositionX += tileSize.width;
		tileHeight = tileSize.height;
		addChild(tile);
	}

	// Add physics body to created figure
	PhysicsBodyConfig config(Size(tilePositionX, tileHeight), PhysicsMaterial(),
		BodyType::Box, DEFAULT_COLLISION_MASK, false, false);
	PhysicsManager::AddPhysicsBody(*this, config);

	return tileHeight > 0 && tilePositionX > 0;
}
