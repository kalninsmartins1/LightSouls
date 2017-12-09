#include "XMLLoader.h"
#include "physics/CCPhysicsBody.h"
#include "GameConsts.h"
#include "Entity/Player/Player.h"
#include "Utils.h"
#include "Entity/Components/PlayerAnimComponent.h"
#include "Entity/Components/MirrorSpriteComponent.h"


using namespace cocos2d;

XMLLoader::XMLLoader()
{
}

bool XMLLoader::initializeSpriteUsingXMLFile(Sprite& sprite, const char* pathToXML)
{
	// Load the file
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(pathToXML);

	bool isInitSuccessful = true;
	if (err)
	{
		cocos2d::log("XMLLoader: [initializeSpriteUsingXMLFile] XML file not found: %s", pathToXML);
		isInitSuccessful = false;
	}
	else
	{
		// Parse the file
		tinyxml2::XMLNode* pData = doc.RootElement();

		for (tinyxml2::XMLNode* pNode = pData->FirstChildElement(); pNode;
			pNode = pNode->NextSibling())
		{
			// Check all actor components
			std::string nodeValue(pNode->ToElement()->Attribute("type"));
			if (Utils::isStrEqual(nodeValue, XML_TRANSFORM_COMPONENT))
			{
				// Trasform component has data types in specific order
				tinyxml2::XMLNode* pPositionNode = pNode->FirstChild();
				tinyxml2::XMLNode* pRotationNode = pPositionNode->NextSibling();

				sprite.setPosition3D(loadVec3FromAttributes(pPositionNode));
				sprite.setRotation3D(loadVec3FromAttributes(pRotationNode));
			}
			else if (Utils::isStrEqual(nodeValue, XML_PLAYER_CONTROLLER_COMPONENT))
			{
				Player* pPlayer = (Player*)&sprite;
				float moveSpeed = pNode->ToElement()->FloatAttribute("moveSpeed");
				pPlayer->setMoveSpeed(moveSpeed);
			}
			else if (Utils::isStrEqual(nodeValue, XML_PLAYER_ANIM_COMPONENT))
			{
				PlayerAnimComponent* pPlayerAnim = PlayerAnimComponent::create();
				pPlayerAnim->setName(XML_PLAYER_ANIM_COMPONENT);
				sprite.addComponent(pPlayerAnim);
				pPlayerAnim->loadConfig(pNode);
			}
			else if (Utils::isStrEqual(nodeValue, XML_RIGID_BODY_COMPONENT))
			{
				PhysicsBody* pPhysicsBody = loadPhysicsBodyFromAttributes(pNode);
				pPhysicsBody->setName(XML_RIGID_BODY_COMPONENT);
				sprite.addComponent(pPhysicsBody);
			}
			else if(Utils::isStrEqual(nodeValue, XML_MIRROR_SPRITE_COMPONENT))
			{
				MirrorSpriteComponent* pMirrorSprite = MirrorSpriteComponent::create();
				pMirrorSprite->setName(XML_MIRROR_SPRITE_COMPONENT);
				sprite.addComponent(pMirrorSprite);
			}
		}
	}
	return isInitSuccessful;
}

PhysicsBody* XMLLoader::loadPhysicsBodyFromAttributes(const tinyxml2::XMLNode* pNode)
{
	const tinyxml2::XMLElement* pPhysicsBodyElem =
		pNode->FirstChildElement(XML_PHYSICS_BODY);
	std::string bodyType = pPhysicsBodyElem->Attribute("shape");
	const Size bodySize = Size(pPhysicsBodyElem->FloatAttribute("width"),
		pPhysicsBodyElem->FloatAttribute("height"));
	const bool isGravityEnabled = pPhysicsBodyElem->BoolAttribute("isGravityEnabled");
	const bool isBodyDynamic = pPhysicsBodyElem->BoolAttribute("isDynamic");
	const int collisionBitMask = pPhysicsBodyElem->IntAttribute("collisionBitMask");

	PhysicsBody* pPhysicsBody = nullptr;
	if (bodyType.compare("box") == 0)
	{
		pPhysicsBody = PhysicsBody::createBox(bodySize,
			loadPhysicsMaterialFromAttributes(pNode));
		pPhysicsBody->setDynamic(isBodyDynamic);
		pPhysicsBody->setGravityEnable(isGravityEnabled);
		pPhysicsBody->setCollisionBitmask(collisionBitMask);
	}
	return pPhysicsBody;
}

PhysicsMaterial XMLLoader::loadPhysicsMaterialFromAttributes(
	const tinyxml2::XMLNode* pNode)
{
	const tinyxml2::XMLElement* pPhysicsMaterialElem = pNode->
		FirstChildElement(XML_PHYSICS_MATERIAL);
	const float density = pPhysicsMaterialElem->FloatAttribute("density");
	const float restitution = pPhysicsMaterialElem->FloatAttribute("restitution");
	const float friction = pPhysicsMaterialElem->FloatAttribute("friction");

	return PhysicsMaterial(density, restitution, friction);
}

Vec3 XMLLoader::loadVec3FromAttributes(const tinyxml2::XMLNode* pNode)
{
	Vec3 result;
	result.x = pNode->ToElement()->FloatAttribute("x");
	result.y = pNode->ToElement()->FloatAttribute("y");
	result.z = pNode->ToElement()->FloatAttribute("z");

	return result;
}