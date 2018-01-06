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
	// Private constructor to prevent instance creation
}

void XMLLoader::loadActionTriggers(GameInput& gameInput, const XMLElement* pElement,
	LoadInputCallback onKeyboardInput, LoadInputCallback onMouseInput, 
	LoadInputCallback onGameControllerInput)
{
	// Get element action
	const char* actionName = pElement->ToElement()->
									Attribute(XML_NAME_ATTR);

	// Get element action triggers
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* childNodeName = pChild->Value();
		if (Utils::isStrEqual(childNodeName, XML_INPUT_KEYBOARD))
		{
			if(onKeyboardInput != nullptr)
			{
				onKeyboardInput(gameInput, pChild, actionName);
			}
		}
		else if (Utils::isStrEqual(childNodeName, XML_INPUT_MOUSE))
		{
			if(onMouseInput != nullptr)
			{
				onMouseInput(gameInput, pChild, actionName);
			}
		}
		else if (Utils::isStrEqual(childNodeName, XML_INPUT_GAME_CONTROLLER))
		{
			if(onGameControllerInput != nullptr)
			{
				onGameControllerInput(gameInput, pChild, actionName);
			}
		}
	}
}

void XMLLoader::loadKeyboardAxis(GameInput& gameInput, const XMLElement* pElement,
	const char* actionName)
{
	// Go trough all keys that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* keyCodeFrom = pChild->Attribute(XML_INPUT_KEY_FROM_ATTR);
		const char* keyCodeTo = pChild->Attribute(XML_INPUT_KEY_TO_ATTR);
		
		const float valueFrom = pChild->FloatAttribute(XML_INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_INPUT_VALUE_TO_ATTR);

		gameInput.addAxisActionInput(GameInputType::Keyboard,
			actionName, keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	}
}

void XMLLoader::loadGameControllerAxis(GameInput& gameInput, const XMLElement* pElement, const char* actionName)
{
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* axisName = pChild->Attribute(XML_INPUT_AXIS_NAME);
		const float valueFrom = pChild->FloatAttribute(XML_INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_INPUT_VALUE_TO_ATTR);

		gameInput.addAxisActionInput(GameInputType::GameController,
			actionName, axisName, axisName, valueFrom, valueTo);
	}
}

void XMLLoader::loadActionButton(GameInput& gameInput, GameInputType inputType, const XMLElement* pElement,
	const char* actionName, const char* xmlAttributeName)
{
	// Go trough all buttons that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* buttonCode = pChild->Attribute(xmlAttributeName);
		if (buttonCode != nullptr)
		{
			gameInput.addActionInput(inputType, actionName, buttonCode);
		}
		else
		{
			CCASSERT(false, "XMLLoader: [loadActionButton] missing button code !");
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

				const float moveSpeed = pMovementElement->FloatAttribute(XML_ENTITY_MOVE_SPEED);
				const float dodgeSpeed = pMovementElement->FloatAttribute(XML_ENTITY_DODGE_SPEED);
				const float dodgeTime = pMovementElement->FloatAttribute(XML_ENTITY_DODGE_TIME);

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
			if (Utils::isStrEqual(nodeName, XML_INPUT_AXIS))
			{
				loadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadKeyboardAxis(gameInput, pElem, actionName);
					},

					nullptr, // Currently not handling Mouse axis input
					
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadGameControllerAxis(gameInput, pElem, actionName);
					});
			}
			else if (Utils::isStrEqual(nodeName, XML_INPUT_ACTION_BUTTON))
			{
				loadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadActionButton(gameInput, GameInputType::Keyboard, pElem, 
							actionName, XML_INPUT_KEY_CODE_ATTR);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadActionButton(gameInput, GameInputType::Mouse, pElem,
							actionName,  XML_INPUT_BUTTON_CODE_ATTR);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const char* actionName)
					{
						loadActionButton(gameInput, GameInputType::GameController, pElem,
							actionName, XML_INPUT_BUTTON_CODE_ATTR);
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
	if (Utils::isStrEqual(bodyType, "box"))
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
	if (Utils::isStrEqual(inputType, XML_INPUT_KEYBOARD))
	{
		type = GameInputType::Keyboard;
	}
	else if (Utils::isStrEqual(inputType, XML_INPUT_MOUSE))
	{
		type = GameInputType::Mouse;
	}
	else if (Utils::isStrEqual(inputType, XML_INPUT_GAME_CONTROLLER))
	{
		type = GameInputType::GameController;
	}
	else
	{
		Utils::assertWithStrFormat(false,
			"XMLLoader: [strToGameInpuType] invalid input type: %s",
			inputType);
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