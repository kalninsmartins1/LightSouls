#include "XMLLoader.h"
#include "physics/CCPhysicsBody.h"
#include "GameConsts.h"
#include "World/Entity/Player/Player.h"
#include "Utils.h"
#include "World/Entity/Components/PlayerAnimComponent.h"
#include "World/Entity/Components/MirrorSpriteComponent.h"
#include "Input/GameInput.h"
#include "World/World.h"


using namespace cocos2d;

XMLLoader::XMLLoader()
{
	// Private constructor to prevent instance creation
}

bool XMLLoader::initializeAIManagerUsingXMLFile(AIAgentManager& aiManager, const std::string& pathToXML)
{
	XMLDoc doc;
	XMLError err = doc.LoadFile(pathToXML.c_str());
	
	if (!err)
	{
		XMLElement* pData = doc.RootElement();
		for (XMLElement* pNode = pData->FirstChildElement(); pNode;
			pNode = pNode->NextSiblingElement())
		{
			const std::string& elementName = pNode->Value();
			if(elementName == "AgentConfigList")
			{
				// Load all agent configs
				for (XMLElement* pChild = pNode->FirstChildElement(); pChild;
					pChild = pChild->NextSiblingElement())
				{
					const std::string& path = pChild->Attribute(XML_PATH_ATTR);
					aiManager.addAgent(AIAgent::create(path));
				}
			}
		}
	}
	else
	{
		Utils::assertWithStrFormat(false, "XMLLoader: Failed to load file from path %s",
			pathToXML);
	}

	return !err;
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
		const std::string& childNodeName = pChild->Value();
		if (childNodeName == XML_INPUT_KEYBOARD)
		{
			if(onKeyboardInput != nullptr)
			{
				onKeyboardInput(gameInput, pChild, actionName);
			}
		}
		else if (childNodeName == XML_INPUT_MOUSE)
		{
			if(onMouseInput != nullptr)
			{
				onMouseInput(gameInput, pChild, actionName);
			}
		}
		else if (childNodeName == XML_INPUT_GAME_CONTROLLER)
		{
			if(onGameControllerInput != nullptr)
			{
				onGameControllerInput(gameInput, pChild, actionName);
			}
		}
	}
}

void XMLLoader::loadKeyboardAxis(GameInput& gameInput, const XMLElement* pElement,
	const std::string& actionName)
{
	// Go trough all keys that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const std::string& keyCodeFrom = pChild->Attribute(XML_INPUT_KEY_FROM_ATTR);
		const std::string& keyCodeTo = pChild->Attribute(XML_INPUT_KEY_TO_ATTR);
		
		const float valueFrom = pChild->FloatAttribute(XML_INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_INPUT_VALUE_TO_ATTR);

		gameInput.addAxisActionInput(GameInputType::Keyboard,
			actionName, keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	}
}

void XMLLoader::loadGameControllerAxis(GameInput& gameInput, const XMLElement* pElement, const std::string& actionName)
{
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const std::string& axisName = pChild->Attribute(XML_INPUT_AXIS_NAME);
		const float valueFrom = pChild->FloatAttribute(XML_INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_INPUT_VALUE_TO_ATTR);

		gameInput.addAxisActionInput(GameInputType::GameController,
			actionName, axisName, axisName, valueFrom, valueTo);
	}
}

void XMLLoader::loadActionButton(GameInput& gameInput, GameInputType inputType, const XMLElement* pElement,
	const std::string& actionName, const std::string& xmlAttributeName)
{
	// Go trough all buttons that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* buttonCode = pChild->Attribute(xmlAttributeName.c_str());
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

bool XMLLoader::initializeSpriteUsingXMLFile(Sprite& sprite, const std::string& pathToXML)
{
	// Load the file
	XMLDoc doc;
	XMLError err = doc.LoadFile(pathToXML.c_str());
	
	if (!err)
	{
		XMLElement* pData = doc.RootElement();
		for (XMLElement* pElement = pData->FirstChildElement(); pElement;
			pElement = pElement->NextSiblingElement())
		{
			// Check all actor components
			const std::string componentType = pElement->Attribute(XML_TYPE_ATTR);
			
			if (componentType == XML_TRANSFORM_COMPONENT)
			{
				// Trasform component has data types in specific order
				XMLNode* pPositionNode = pElement->FirstChild();
				XMLNode* pRotationNode = pPositionNode->NextSibling();

				sprite.setPosition3D(loadVec3FromAttributes(pPositionNode));
				sprite.setRotation3D(loadVec3FromAttributes(pRotationNode));
			}
			else if (componentType == XML_PLAYER_CONTROLLER_COMPONENT)
			{
				const XMLElement* pMovementElement = pElement->FirstChildElement();

				const float moveSpeed = pMovementElement->FloatAttribute(XML_ENTITY_MOVE_SPEED);
				const float dodgeSpeed = pMovementElement->FloatAttribute(XML_ENTITY_DODGE_SPEED);
				const float dodgeTime = pMovementElement->FloatAttribute(XML_ENTITY_DODGE_TIME);
				const float timeBetweenComboInput =
					pMovementElement->FloatAttribute("timeBetweenComboInput");

				Player* pPlayer = dynamic_cast<Player*>(&sprite);
				pPlayer->setMoveSpeed(moveSpeed);
				pPlayer->setDodgeSpeed(dodgeSpeed);
				pPlayer->setDodgeTime(dodgeTime);
				pPlayer->setTimeBetweenComboInput(timeBetweenComboInput);
			}
			else if(componentType == XML_AI_CONTROLLER_COMPONENT)
			{
				const float attackRadius = pElement->FloatAttribute("attackRadius");
				const float workingRadius = pElement->FloatAttribute("workingRadius");
				const Vec3& basePosition = 
					loadVec3FromAttributes(pElement->FirstChildElement("BasePosition"));

				AIAgent* pAgent = dynamic_cast<AIAgent*>(&sprite);
				pAgent->setBasePosition(Vec2(basePosition.x, basePosition.y));
				pAgent->setAttackRadius(attackRadius);
				pAgent->setWorkingRadius(workingRadius);
			}
			else if (componentType == XML_PLAYER_ANIM_COMPONENT)
			{
				PlayerAnimComponent* pPlayerAnim = PlayerAnimComponent::create(sprite);
				pPlayerAnim->setName(XML_PLAYER_ANIM_COMPONENT);
				sprite.addComponent(pPlayerAnim);
				pPlayerAnim->loadConfig(pElement);
			}
			else if (componentType == XML_RIGID_BODY_COMPONENT)
			{
				PhysicsBody* pPhysicsBody = loadPhysicsBodyFromAttributes(pElement);
				pPhysicsBody->setName(XML_RIGID_BODY_COMPONENT);
				sprite.addComponent(pPhysicsBody);
			}
			else if (componentType == XML_MIRROR_SPRITE_COMPONENT)
			{
				MirrorSpriteComponent* pMirrorSprite = MirrorSpriteComponent::create();
				pMirrorSprite->setName(XML_MIRROR_SPRITE_COMPONENT);
				pMirrorSprite->setPlayer(dynamic_cast<Player*>(&sprite));
				sprite.addComponent(pMirrorSprite);
			}
		}
	}
	return !err;
}

bool XMLLoader::loadInputSettings(GameInput& gameInput, const std::string& pathToConfigXml)
{
	// Load the file
	XMLDoc doc;
	const XMLError err = doc.LoadFile(pathToConfigXml.c_str());
	if(!err)
	{
		const XMLElement* pRootNode = doc.RootElement();
		const XMLElement* pFirstElement = pRootNode->FirstChildElement();

		// Load all input configurations
		for (const XMLElement* pElem = pFirstElement; pElem != nullptr;
			pElem = pElem->NextSiblingElement())
		{
			const char* nodeName = pElem->Value();
			if (nodeName == XML_INPUT_AXIS)
			{
				loadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadKeyboardAxis(gameInput, pElem, actionName);
					},

					nullptr, // Currently not handling Mouse axis input
					
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadGameControllerAxis(gameInput, pElem, actionName);
					});
			}
			else if (nodeName == XML_INPUT_ACTION_BUTTON)
			{
				loadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadActionButton(gameInput, GameInputType::Keyboard, pElem, 
							actionName, XML_INPUT_KEY_CODE_ATTR);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadActionButton(gameInput, GameInputType::Mouse, pElem,
							actionName,  XML_INPUT_BUTTON_CODE_ATTR);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadActionButton(gameInput, GameInputType::GameController, pElem,
							actionName, XML_INPUT_BUTTON_CODE_ATTR);
					});
			}
		}
	}	

	return !err;
}

bool XMLLoader::loadWorld(World& world, const std::string& pathToXML)
{
	XMLDoc doc;
	const XMLError error = doc.LoadFile(pathToXML.c_str());
	if (!error)
	{
		XMLElement* pRoot = doc.RootElement();
		XMLElement* pChild = pRoot->FirstChildElement();
		const char* pathToSprite = pChild->Attribute("path");
		world.initWithFile(pathToSprite);
	}
	else
	{
		Utils::assertWithStrFormat(false,
			"XMLLoader: Failed to load .xml file at %s", pathToXML.c_str());
	}

	return !error;
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
	if (bodyType == "box")
	{
		pPhysicsBody = PhysicsBody::createBox(bodySize,
			loadPhysicsMaterialFromAttributes(pNode));
		pPhysicsBody->setDynamic(isBodyDynamic);
		pPhysicsBody->setGravityEnable(isGravityEnabled);
		pPhysicsBody->setCollisionBitmask(collisionBitMask);
	}
	return pPhysicsBody;
}

GameInputType XMLLoader::strToGameInputType(const std::string& inputType)
{
	GameInputType type = GameInputType::None;
	if (inputType == XML_INPUT_KEYBOARD)
	{
		type = GameInputType::Keyboard;
	}
	else if (inputType == XML_INPUT_MOUSE)
	{
		type = GameInputType::Mouse;
	}
	else if (inputType == XML_INPUT_GAME_CONTROLLER)
	{
		type = GameInputType::GameController;
	}
	else
	{
		Utils::assertWithStrFormat(false,
			"XMLLoader: [strToGameInpuType] invalid input type: %s",
			inputType.c_str());
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