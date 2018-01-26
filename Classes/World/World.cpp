#include "World.h"
#include "Utils/XML/XMLLoader.h"

World* World::create(const char* pathToXML)
{
	World* pWorld = new (std::nothrow) World();
	if (pWorld && pWorld->init(pathToXML))
	{
		pWorld->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pWorld);
	}

	return pWorld;
}

bool World::init(const char* pathToXML)
{
	return XMLLoader::loadWorld(*this, pathToXML);
}
