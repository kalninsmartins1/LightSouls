#include "XMLLoader.h"
#include "physics/CCPhysicsBody.h"
#include "GameConsts.h"
#include "Entity/Player/Player.h"
#include "Utils.h"
#include "Entity/Components/PlayerAnimComponent.h"
#include "Entity/Components/MirrorSpriteComponent.h"
#include "Input/GameInput.h"


using namespace cocos2d;

XMLLoader::XMLLoader()
{
}

void XMLLoader::loadActionTriggers(GameInput& gameInput, const XMLElement* pElement,
	LoadInputCallback onKeyboardInput, LoadInputCallback onMouseInput)
{
	// Get element action
	const char* actionName = pElement->ToElement()->
									Attribute(XML_NAME_ATTR);

	// Get element action triggers
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* childNodeName = pChild->Value();
		if (Utils::isStrEqual(childNodeName, "Keyboard"))
		{
			if(onKeyboardInput != nullptr)
			{
				onKeyboardInput(gameInput, pChild, actionName);
			}			
		}
		else if (Utils::isStrEqual(childNodeName, "Mouse"))
		{
			if(onKeyboardInput != nullptr)
			{
				onMouseInput(gameInput, pChild, actionName);
			}
		}
		else if (Utils::isStrEqual(childNodeName, "Joystick"))
		{
			// Currently not handling joystick input
		}
	}
}

void XMLLoader::loadKeyboardAxis(GameInput& gameInput, const XMLElement* pElement,
	const char* actionName)
{
	const char* inputTypeStr = pElement->Value();
	GameInputType inputType = strToGameInputType(inputTypeStr);

	// Go trough all keys that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* keyCodeFrom = pChild->Attribute(XML_KEY_CODE_FROM_ATTR);
		const char* keyCodeTo = pChild->Attribute(XML_KEY_CODE_TO_ATTR);
		
		const float valueFrom = pChild->FloatAttribute(XML_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_VALUE_TO_ATTR);

		gameInput.addAxisActionInput(inputType, actionName, keyCodeFrom, keyCodeTo,
			valueFrom, valueTo);
	}
}

void XMLLoader::loadMouseButton(GameInput& gameInput, const XMLElement* pElement,
	const char* actionName)
{
	const char* inputTypeStr = pElement->Value();
	GameInputType inputType = strToGameInputType(inputTypeStr);

	// Go trough all keys that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* buttonCode = pChild->Attribute(XML_BUTTON_CODE_ATTR);
		if(buttonCode != nullptr)
		{
			gameInput.addActionInput(inputType, actionName, buttonCode);
		}
		else
		{
			cocos2d::log("XMLLoader: [loadMouseButton] button code does not exist !");
		}
	}
}

void XMLLoader::loadActionKeyButton(GameInput& gameInput, const XMLElement* pElement,
	const char* actionName)
{
	const char* inputTypeStr = pElement->Value();
	GameInputType inputType = strToGameInputType(inputTypeStr);

	// Go trough all keys that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* keyCode = pChild->Attribute(XML_KEY_CODE_ATTR);
		if(keyCode != nullptr)
		{
			gameInput.addActionInput(inputType, actionName, keyCode);
		}
		else
		{
			cocos2d::log("XMLLoader: [loadActionKeyButton] key code does not exist !");
		}
	}
}

bool XMLLoader::initializeSpriteUsingXMLFile(Sprite& sprite, const char* pathToXML)
{
	// Load the file	
	XMLDoc doc;
	XMLError err = doc.LoadFile(pathToXML);
	
	if (!err)
	{
		XMLNode* pData = doc.RootElement();
		for (XMLNode* pNode = pData->FirstChildElement(); pNode;
			pNode = pNode->NextSibling())
		{
			// Check all actor components
			std::string nodeValue(pNode->ToElement()->
									Attribute(XML_TYPE_ATTR));
			if (Utils::isStrEqual(nodeValue, XML_TRANSFORM_COMPONENT))
			{
				// Trasform component has data types in specific order
				XMLNode* pPositionNode = pNode->FirstChild();
				XMLNode* pRotationNode = pPositionNode->NextSibling();

				sprite.setPosition3D(loadVec3FromAttributes(pPositionNode));
				sprite.setRotation3D(loadVec3FromAttributes(pRotationNode));
			}
			else if (Utils::isStrEqual(nodeValue, XML_PLAYER_CONTROLLER_COMPONENT))
			{
				Player* pPlayer = (Player*)&sprite;
				const XMLElement* pMovementElement = pNode->FirstChildElement();

				float moveSpeed = pMovementElement->FloatAttribute("moveSpeed");
				float dodgeSpeed = pMovementElement->FloatAttribute("dodgeSpeed");
				float dodgeTime = pMovementElement->FloatAttribute("dodgeTime");

				pPlayer->setMoveSpeed(moveSpeed);
				pPlayer->setDodgeSpeed(dodgeSpeed);
				pPlayer->setDodgeTime(dodgeTime);
			}
			else if (Utils::isStrEqual(nodeValue, XML_PLAYER_ANIM_COMPONENT))
			{
				PlayerAnimComponent* pPlayerAnim = PlayerAnimComponent::create();
				pPlayerAnim->setName(XML_PLAYER_ANIM_COMPONENT);
				pPlayerAnim->setSprite(&sprite);
				sprite.addComponent(pPlayerAnim);
				pPlayerAnim->loadConfig(pNode);
			}
			else if (Utils::isStrEqual(nodeValue, XML_RIGID_BODY_COMPONENT))
			{
				PhysicsBody* pPhysicsBody = loadPhysicsBodyFromAttributes(pNode);
				pPhysicsBody->setName(XML_RIGID_BODY_COMPONENT);
				sprite.addComponent(pPhysicsBody);
			}
			else if (Utils::isStrEqual(nodeValue, XML_MIRROR_SPRITE_COMPONENT))
			{
				MirrorSpriteComponent* pMirrorSprite = MirrorSpriteComponent::create();
				pMirrorSprite->setName(XML_MIRROR_SPRITE_COMPONENT);
				pMirrorSprite->setPlayer((Player*)&sprite);
				sprite.addComponent(pMirrorSprite);
			}
		}		
	}
	return !err;
}

bool XMLLoader::loadInputSettings(GameInput& gameInput, const char* pathToConfigXml)
{
	// Load the file	
	XMLDoc doc;
	const XMLError err = doc.LoadFile(pathToConfigXml);
	if(!err)
	{
		const XMLElement* pRootNode = doc.RootElement();		
		const XMLElement* pFirstElement = pRootNode->FirstChildElement();

		// Load all input configurations
		for (const XMLElement* pElem = pFirstElement; pElem != nullptr;
			pElem = pElem->NextSiblingElement())
		{
			const char* nodeName = pElem->Value();
			if (Utils::isStrEqual(nodeName, "Axis"))
			{
				loadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadKeyboardAxis(gameInput, pElem, actionName);
					},
					nullptr); // Currently not handling Mouse axis input
			}
			else if (Utils::isStrEqual(nodeName, "ActionButton"))
			{
				loadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadActionKeyButton(gameInput, pElem, actionName);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadMouseButton(gameInput, pElem, actionName);
					});
			}
		}
	}
	

	return !err;
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

GameInputType XMLLoader::strToGameInputType(const char * inputType)
{
	GameInputType type = GameInputType::None;
	if (Utils::isStrEqual(inputType, KEYBOARD_INPUT))
	{
		type = GameInputType::Keyboard;
	}
	else if (Utils::isStrEqual(inputType, MOUSE_INPUT))
	{
		type = GameInputType::Mouse;
	}
	else if (Utils::isStrEqual(inputType, JOYSTICK_INPUT))
	{
		type = GameInputType::Joystick;
	}
	else
	{
		cocos2d::log("XMLLoader: [strToGameInpuType] invalid input type: %s", inputType);
	}
	return type;
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