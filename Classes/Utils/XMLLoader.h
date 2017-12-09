#pragma once
#include "cocos2d.h"

namespace tinyxml2 
{
	class XMLNode;
}

class XMLLoader
{
public:
	static bool XMLLoader::initializeSpriteUsingXMLFile(cocos2d::Sprite& sprite,
		const char* pathToXML);
	

private:
	XMLLoader();

	static cocos2d::PhysicsMaterial loadPhysicsMaterialFromAttributes(
		const tinyxml2::XMLNode* pNode);
	static cocos2d::Vec3 XMLLoader::loadVec3FromAttributes(
		const tinyxml2::XMLNode* pNode);
	static cocos2d::PhysicsBody* XMLLoader::loadPhysicsBodyFromAttributes(
		const tinyxml2::XMLNode* pNode);
};
