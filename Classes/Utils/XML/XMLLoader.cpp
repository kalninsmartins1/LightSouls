#include "XMLLoader.h"
#include "physics/CCPhysicsBody.h"
#include "XMLConsts.h"
#include "World/Entity/Player/Player.h"
#include "../Utils.h"
#include "World/Entity/Components/PlayerAnimComponent.h"
#include "World/Entity/Components/MirrorSpriteComponent.h"
#include "Input/GameInput.h"
#include "World/World.h"
#include "World/Entity/Components/AIAnimComponent.h"
#include "GameConsts.h"
#include "World/Entity/Components/RangedAttackComponent.h"
#include <rapidxml/rapidxml.hpp>
#include "World/Entity/Components/LongSwordAttackComponent.h"

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
			if(elementName == XML_NODE_AGENT_CONFIG_LIST)
			{
				// Load all agent configurations
				for (XMLElement* pChild = pNode->FirstChildElement(); pChild;
					pChild = pChild->NextSiblingElement())
				{
					const std::string& path = pChild->Attribute(XML_PATH_ATTR);
					const std::string& type = pChild->Attribute(XML_TYPE_ATTR);
					aiManager.addAgentConfig(type, path);
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

		gameInput.addAxisActionInput(GameInputType::KEYBOARD,
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

		gameInput.addAxisActionInput(GameInputType::GAME_CONTROLLER,
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

bool XMLLoader::initializeEntityUsingXMLFile(Entity& entity, 
	const std::string& pathToXML)
{
	// Load the file
	XMLDoc doc;
	const XMLError err = doc.LoadFile(pathToXML.c_str());
	
	if (!err)
	{
		XMLElement* pRoot = doc.RootElement();

		// Load root entity atributes
		const std::string actorType = pRoot->Attribute(XML_TYPE_ATTR);
		const float moveSpeed = pRoot->FloatAttribute(XML_ENTITY_MOVE_SPEED_ATTR);
		const float dodgeSpeed = pRoot->FloatAttribute(XML_ENTITY_DODGE_SPEED_ATTR);
		const float dodgeTime = pRoot->FloatAttribute(XML_ENTITY_DODGE_TIME_ATTR);
		const float baseHealth = pRoot->FloatAttribute(XML_ENTITY_BASE_HEALTH_ATTR);
		const float baseDamage = pRoot->FloatAttribute(XML_ENTITY_BASE_DAMAGE_ATTR);

		entity.setName(actorType);
		entity.setBaseMoveSpeed(moveSpeed);
		entity.setDodgeSpeed(dodgeSpeed);
		entity.setDodgeTime(dodgeTime);
		entity.setBaseDamage(baseDamage);
		entity.setBaseHealth(baseHealth);

		// Load entity components
		for (XMLElement* pElement = pRoot->FirstChildElement(); pElement;
			pElement = pElement->NextSiblingElement())
		{
			// Check all actor components
			const std::string componentType = pElement->Attribute(XML_TYPE_ATTR);
			
			if (componentType == TRANSFORM_COMPONENT)
			{
				// Trasform component has data types in specific order
				XMLNode* pPositionNode = pElement->FirstChild();
				XMLNode* pRotationNode = pPositionNode->NextSibling();
				XMLNode* pScaleNode = pRotationNode->NextSiblingElement();

				entity.setPosition3D(loadVec3FromAttributes(pPositionNode));
				entity.setRotation3D(loadVec3FromAttributes(pRotationNode));

				const Vec3 scale = loadVec3FromAttributes(pScaleNode);
				entity.setScale(scale.x, scale.y);
			}
			else if (componentType == PLAYER_CONTROLLER_COMPONENT)
			{
				const float timeBetweenComboInput =
					pElement->FloatAttribute(XML_PLAYER_TIME_BETWEEN_COMBO_HIT_ATTR);

				Player* pPlayer = dynamic_cast<Player*>(&entity);
				pPlayer->setTimeBetweenComboInput(timeBetweenComboInput);
			}
			else if(componentType == AI_CONTROLLER_COMPONENT)
			{
				const float attackRadius = pElement->FloatAttribute(XML_AI_ATTACK_RADIUS_ATTR);
				const float workingRadius = pElement->FloatAttribute(XML_AI_WORKING_RADIUS_ATTR);
				const float patrolPause = pElement->FloatAttribute(XML_AI_PATROL_PAUSE_ATTR);

				AIAgent* pAgent = dynamic_cast<AIAgent*>(&entity);
				const std::string& agentType = pRoot->Attribute(XML_TYPE_ATTR);
				pAgent->setAgentType(agentType);
				pAgent->setAttackRadius(attackRadius);
				pAgent->setWorkingRadius(workingRadius);
				pAgent->setPatrolPause(patrolPause);
			}
			else if (componentType == PLAYER_ANIM_COMPONENT)
			{
				PlayerAnimComponent* pPlayerAnim = PlayerAnimComponent::create(entity);
				pPlayerAnim->setName(PLAYER_ANIM_COMPONENT);
				pPlayerAnim->loadConfig(pElement);
				entity.addComponent(pPlayerAnim);
			}
			else if(componentType == AI_ANIM_COMPONENT)
			{
				AIAnimComponent* pAgentAnim = AIAnimComponent::create(entity);
				pAgentAnim->setName(AI_ANIM_COMPONENT);
				pAgentAnim->loadConfig(pElement);
				entity.addComponent(pAgentAnim);
			}
			else if(componentType == RANGED_ATTACK_COMPONENT)
			{
				const std::string& pathToAmmoSprite =
					pElement->Attribute(XML_PATH_ATTR);
				const float maxAmmoFlyDistance =
					pElement->FloatAttribute(XML_AI_MAX_FLY_DISTANCE);
				const float ammoMoveSpeed = 
					pElement->FloatAttribute(XML_ENTITY_MOVE_SPEED_ATTR);
				const float secondsBetweenAttacks =
					pElement->FloatAttribute(
						XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);

				RangedAttackComponent* pRangedAttack = 
					RangedAttackComponent::create(
						pathToAmmoSprite,
						maxAmmoFlyDistance,
						ammoMoveSpeed,
						secondsBetweenAttacks);
				pRangedAttack->setName(ATTACK_COMPONENT);
				entity.addComponent(pRangedAttack);
			}
			else if(componentType == LONG_SWORD_ATTACK_COMPONENT)
			{				
				const float secondsBetweenAttacks =
					pElement->FloatAttribute(
						XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
				const float attackRange = pElement->FloatAttribute(XML_ENTITY_ATTACK_RANGE_ATTR);
				const float paddingFromBody = pElement->
					FloatAttribute(XML_ENTITY_PADDING_FROM_BODY_ATTR);

				LongSwordAttackComponent* pLongSwordAttack =
					LongSwordAttackComponent::create(secondsBetweenAttacks, attackRange,
						paddingFromBody);
				pLongSwordAttack->setName(ATTACK_COMPONENT);
				entity.addComponent(pLongSwordAttack);
			}
			else if (componentType == RIGID_BODY_COMPONENT)
			{
				Size outBodySize;
				PhysicsBody* pPhysicsBody = 
					loadPhysicsBodyFromAttributes(pElement, outBodySize);
				pPhysicsBody->setName(RIGID_BODY_COMPONENT);
				entity.addComponent(pPhysicsBody);
				static_cast<Entity*>(&entity)->setPhysicsBodySize(outBodySize);
			}
			else if (componentType == MIRROR_SPRITE_COMPONENT)
			{
				MirrorSpriteComponent* pMirrorSprite = MirrorSpriteComponent::create();
				pMirrorSprite->setName(MIRROR_SPRITE_COMPONENT);
				pMirrorSprite->setOwnerEntity(dynamic_cast<Entity*>(&entity));
				entity.addComponent(pMirrorSprite);
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
			const std::string& nodeName = pElem->Value();
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
						loadActionButton(gameInput, GameInputType::KEYBOARD, pElem, 
							actionName, XML_INPUT_KEY_CODE_ATTR);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadActionButton(gameInput, GameInputType::MOUSE, pElem,
							actionName,  XML_INPUT_BUTTON_CODE_ATTR);
					},
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
					{
						loadActionButton(gameInput, GameInputType::GAME_CONTROLLER, pElem,
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
		const char* pathToSprite = pChild->Attribute(XML_PATH_ATTR);
		world.initWithFile(pathToSprite);
	}
	else
	{
		Utils::assertWithStrFormat(false,
			"XMLLoader: Failed to load .xml file at %s", pathToXML);
	}

	return !error;
}

PhysicsBody* XMLLoader::loadPhysicsBodyFromAttributes(const tinyxml2::XMLNode* pNode,
	Size& outSize)
{
	const tinyxml2::XMLElement* pPhysicsBodyElem =
		pNode->FirstChildElement(XML_NODE_PHYSICS_BODY);

	const std::string bodyType = pPhysicsBodyElem->Attribute(XML_SHAPE_ATTR);
	outSize = Size(pPhysicsBodyElem->FloatAttribute(XML_WIDTH_ATTR),
		pPhysicsBodyElem->FloatAttribute(XML_HEIGHT_ATTR));

	const bool isGravityEnabled = pPhysicsBodyElem->BoolAttribute(XML_PHYSICS_GRAVITY_ATTR);
	const bool isBodyDynamic = pPhysicsBodyElem->BoolAttribute(XML_PHYSICS_DYNAMIC_BODY_ATTR);
	const int collisionBitMask = pPhysicsBodyElem->IntAttribute(XML_PHYSICS_BIT_MASK_ATTR);

	PhysicsBody* pPhysicsBody = nullptr;
	if (bodyType == XML_PHYSICS_BODY_BOX_ATTR)
	{
		pPhysicsBody = PhysicsBody::createBox(outSize,
			loadPhysicsMaterialFromAttributes(pNode));
		pPhysicsBody->setDynamic(isBodyDynamic);
		pPhysicsBody->setGravityEnable(isGravityEnabled);
		pPhysicsBody->setContactTestBitmask(collisionBitMask); // TODO: Need hex support
		pPhysicsBody->setCategoryBitmask(collisionBitMask);
	}
	return pPhysicsBody;
}

GameInputType XMLLoader::strToGameInputType(const std::string& inputType)
{
	GameInputType type = GameInputType::NONE;
	if (inputType == XML_INPUT_KEYBOARD)
	{
		type = GameInputType::KEYBOARD;
	}
	else if (inputType == XML_INPUT_MOUSE)
	{
		type = GameInputType::MOUSE;
	}
	else if (inputType == XML_INPUT_GAME_CONTROLLER)
	{
		type = GameInputType::GAME_CONTROLLER;
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
		FirstChildElement(XML_NODE_PHYSICS_MATERIAL);
	const float density = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_DESITY_ATTR);
	const float restitution = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_RESTITUTION_ATTR);
	const float friction = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_FRICTION_ATTR);

	return {density, restitution, friction};
}

Vec3 XMLLoader::loadVec3FromAttributes(const tinyxml2::XMLNode* pNode)
{
	Vec3 result;
	result.x = pNode->ToElement()->FloatAttribute(XML_AXIS_X_ATTR);
	result.y = pNode->ToElement()->FloatAttribute(XML_AXIS_Y_ATTR);
	result.z = pNode->ToElement()->FloatAttribute(XML_AXIS_Z_ATTR);

	return result;
}