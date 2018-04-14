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
#include "World/Entity/Components/LongSwordAttackComponent.h"
#include "World/Physics/PhysicsBodyConfig.h"
#include "World/Physics/PhysicsManager.h"
#include "UI/InGameIndicators/ProgressBar.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "UI/UIElementConfig.h"

using namespace cocos2d;

XMLLoader::XMLLoader()
{
	// Private constructor to prevent instance creation
}

bool XMLLoader::LoadXMLFile(const std::string& pathToXML, XMLDoc& outDoc)
{
	XMLError error = outDoc.LoadFile(pathToXML.c_str());

	if (error != XMLError::XML_NO_ERROR)
	{
		Utils::AssertWithStrFormat(false, "XMLLoader: Failed to load file from path %s",
			pathToXML);
	}

	return !error;
}

bool XMLLoader::InitializeAIManagerUsingXMLFile(AIAgentManager& aiManager, const std::string& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* pData = doc.RootElement();
		for (XMLElement* pNode = pData->FirstChildElement(); pNode;
			pNode = pNode->NextSiblingElement())
		{
			const std::string& elementName = pNode->Value();
			if (elementName == XML_NODE_AGENT_CONFIG_LIST)
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

	return isSuccessful;
}

void XMLLoader::LoadActionTriggers(GameInput& gameInput, const XMLElement* pElement,
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
			if (onKeyboardInput != nullptr)
			{
				onKeyboardInput(gameInput, pChild, actionName);
			}
		}
		else if (childNodeName == XML_INPUT_MOUSE)
		{
			if (onMouseInput != nullptr)
			{
				onMouseInput(gameInput, pChild, actionName);
			}
		}
		else if (childNodeName == XML_INPUT_GAME_CONTROLLER)
		{
			if (onGameControllerInput != nullptr)
			{
				onGameControllerInput(gameInput, pChild, actionName);
			}
		}
	}
}

void XMLLoader::LoadKeyboardAxis(GameInput& gameInput, const XMLElement* pElement,
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

void XMLLoader::LoadGameControllerAxis(GameInput& gameInput, const XMLElement* pElement, const std::string& actionName)
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

void XMLLoader::LoadActionButton(GameInput& gameInput, GameInputType inputType, const XMLElement* pElement,
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

void XMLLoader::LoadUIElement(const XMLElement* element, UIElementConfig& outUIElement)
{
	const XMLElement* spriteNode = element->FirstChildElement(XML_NODE_SPRITE);
	const XMLElement* anchorPositionNode = element->FirstChildElement(XML_NODE_NORMALIZED_POSITION);
	const XMLElement* normalizedPositionNode = element->FirstChildElement(XML_NODE_NORMALIZED_POSITION);
	const XMLElement* scaleNode = element->FirstChildElement(XML_NODE_SCALE);

	const std::string& pathToSprite = spriteNode->Attribute(XML_PATH_ATTR);
	
	Vec2 anchorPos;
	LoadVec2FromAttributes(anchorPositionNode, anchorPos);

	Vec2 normalizedPos;
	LoadVec2FromAttributes(normalizedPositionNode, normalizedPos);

	Vec2 scale;
	LoadVec2FromAttributes(scaleNode, scale);

	outUIElement.SetPathToSprite(pathToSprite);
	outUIElement.SetAnchorPosition(anchorPos);
	outUIElement.SetNormalizedPosition(normalizedPos);
	outUIElement.SetScale(scale);
}

bool XMLLoader::InitializeEntityUsingXMLFile(Entity& entity,
	const std::string& pathToXML)
{
	// Load the file
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);

	if (isSuccessful)
	{
		XMLElement* pRoot = doc.RootElement();

		// Load root entity attributes
		const std::string actorType = pRoot->Attribute(XML_TYPE_ATTR);
		const float moveSpeed = pRoot->FloatAttribute(XML_ENTITY_MOVE_SPEED_ATTR);
		const float dodgeSpeed = pRoot->FloatAttribute(XML_ENTITY_DODGE_SPEED_ATTR);
		const float dodgeTime = pRoot->FloatAttribute(XML_ENTITY_DODGE_TIME_ATTR);
		const float baseHealth = pRoot->FloatAttribute(XML_ENTITY_BASE_HEALTH_ATTR);
		const float baseDamage = pRoot->FloatAttribute(XML_ENTITY_BASE_DAMAGE_ATTR);

		entity.setName(actorType);
		entity.SetBaseMoveSpeed(moveSpeed);
		entity.SetDodgeSpeed(dodgeSpeed);
		entity.SetDodgeTime(dodgeTime);
		entity.SetBaseDamage(baseDamage);
		entity.SetBaseHealth(baseHealth);

		// Load entity components
		for (XMLElement* element = pRoot->FirstChildElement(); element;
			element = element->NextSiblingElement())
		{
			// Check all actor components
			const std::string componentType = element->Attribute(XML_TYPE_ATTR);

			if (componentType == TRANSFORM_COMPONENT)
			{
				// Transform component has data types in specific order
				XMLElement* positionNode = element->FirstChildElement(XML_NODE_POSITION);
				XMLElement* rotationNode = element->FirstChildElement(XML_NODE_ROTATION);
				XMLElement* scaleNode = element->FirstChildElement(XML_NODE_SCALE);

				Vec3 position;
				LoadVec3FromAttributes(positionNode, position);

				Vec3 rotation;
				LoadVec3FromAttributes(rotationNode, rotation);

				Vec3 scale;
				LoadVec3FromAttributes(scaleNode, scale);

				entity.setPosition3D(position);
				entity.setRotation3D(rotation);
				entity.setScale(scale.x, scale.y);
			}
			else if (componentType == PLAYER_CONTROLLER_COMPONENT)
			{
				const float timeBetweenComboInput =
					element->FloatAttribute(XML_PLAYER_TIME_BETWEEN_COMBO_HIT_ATTR);

				Player* pPlayer = dynamic_cast<Player*>(&entity);
				pPlayer->SetTimeBetweenComboInput(timeBetweenComboInput);
			}
			else if (componentType == AI_CONTROLLER_COMPONENT)
			{
				const float chaseRadius = element->FloatAttribute(XML_AI_CHASE_RADIUS_ATTR);
				const float chaseStopDistance = element->FloatAttribute(XML_AI_CHASE_STOP_DISTANCE);
				const float patrolRadius = element->FloatAttribute(XML_AI_PATROL_RADIUS_ATTR);
				const float patrolPause = element->FloatAttribute(XML_AI_PATROL_PAUSE_ATTR);

				CCASSERT(chaseRadius > 1, "AI chase radius is too small !");
				CCASSERT(chaseStopDistance > 0, "AI chase distance cant be negative !");
				CCASSERT(patrolRadius> 1, "AI patrol radius is too small !");

				AIAgent* agent = static_cast<AIAgent*>(&entity);
				const std::string& agentType = pRoot->Attribute(XML_TYPE_ATTR);
				agent->SetAgentType(agentType);
				agent->SetChaseRadius(chaseRadius);
				agent->SetChaseStopDistance(chaseStopDistance);
				agent->SetPatrolRadius(patrolRadius);
				agent->SetPatrolPause(patrolPause);
			}
			else if (componentType == PLAYER_ANIM_COMPONENT)
			{
				PlayerAnimComponent* playerAnim = PlayerAnimComponent::create(entity);
				playerAnim->setName(PLAYER_ANIM_COMPONENT);
				playerAnim->LoadConfig(element);
				entity.addComponent(playerAnim);
			}
			else if (componentType == AI_ANIM_COMPONENT)
			{
				AIAnimComponent* pAgentAnim = AIAnimComponent::Create(entity);
				pAgentAnim->setName(AI_ANIM_COMPONENT);
				pAgentAnim->LoadConfig(element);
				entity.addComponent(pAgentAnim);
			}
			else if (componentType == RANGED_ATTACK_COMPONENT)
			{
				const std::string& pathToAmmoSprite =
					element->Attribute(XML_PATH_ATTR);
				const float maxAmmoFlyDistance =
					element->FloatAttribute(XML_AI_MAX_FLY_DISTANCE);
				const float ammoMoveSpeed =
					element->FloatAttribute(XML_ENTITY_MOVE_SPEED_ATTR);
				const float secondsBetweenAttacks =
					element->FloatAttribute(
						XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);

				RangedAttackComponent* pRangedAttack =
					RangedAttackComponent::Create(
						pathToAmmoSprite,
						maxAmmoFlyDistance,
						ammoMoveSpeed,
						secondsBetweenAttacks);
				pRangedAttack->setName(ATTACK_COMPONENT);
				entity.addComponent(pRangedAttack);
			}
			else if (componentType == LONG_SWORD_ATTACK_COMPONENT)
			{
				const float secondsBetweenAttacks =
					element->FloatAttribute(
						XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
				const float attackRange = element->FloatAttribute(XML_ENTITY_ATTACK_RANGE_ATTR);
				const float paddingFromBody = element->
					FloatAttribute(XML_ENTITY_PADDING_FROM_BODY_ATTR);

				LongSwordAttackComponent* pLongSwordAttack =
					LongSwordAttackComponent::Create(secondsBetweenAttacks, attackRange,
						paddingFromBody);
				pLongSwordAttack->setName(ATTACK_COMPONENT);
				entity.addComponent(pLongSwordAttack);
			}
			else if (componentType == RIGID_BODY_COMPONENT)
			{
				Size outBodySize;
				CreatePhysicsBodyFromAttributes(entity, element, outBodySize);
				entity.SetPhysicsBodySize(outBodySize);
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
	return isSuccessful;
}

bool XMLLoader::LoadInputSettings(GameInput& gameInput, const std::string& pathToConfigXml)
{
	// Load the file
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToConfigXml, doc);
	if (isSuccessful)
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
				LoadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
				{
					LoadKeyboardAxis(gameInput, pElem, actionName);
				},

					nullptr, // Currently not handling Mouse axis input

					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
				{
					LoadGameControllerAxis(gameInput, pElem, actionName);
				});
			}
			else if (nodeName == XML_INPUT_ACTION_BUTTON)
			{
				LoadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
				{
					LoadActionButton(gameInput, GameInputType::KEYBOARD, pElem,
						actionName, XML_INPUT_KEY_CODE_ATTR);
				},
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
				{
					LoadActionButton(gameInput, GameInputType::MOUSE, pElem,
						actionName, XML_INPUT_BUTTON_CODE_ATTR);
				},
					[](GameInput& gameInput, const XMLElement* pElem, const std::string& actionName)
				{
					LoadActionButton(gameInput, GameInputType::GAME_CONTROLLER, pElem,
						actionName, XML_INPUT_BUTTON_CODE_ATTR);
				});
			}
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeUIProgressBar(ProgressBar& healthBar, const std::string& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		const XMLElement* rootNode = doc.RootElement();
		const XMLElement* bgNode = rootNode->FirstChildElement(XML_NODE_BACKGROUND);
		const XMLElement* fgNode = rootNode->FirstChildElement(XML_NODE_FOREGROUND);

		UIElementConfig bgConfig;
		LoadUIElement(bgNode, bgConfig);

		UIElementConfig fgConfig;
		LoadUIElement(fgNode, fgConfig);

		float animationSpeed = rootNode->FloatAttribute(XML_UI_ANIMATION_SPEED_ATTR);
		float minValue = rootNode->FloatAttribute(XML_UI_MIN_VALUE);
		float maxValue = rootNode->FloatAttribute(XML_UI_MAX_VALUE);

		healthBar.Init(bgConfig, fgConfig);
		healthBar.SetAnimationSpeed(animationSpeed);
	}

	return isSuccessful;
}

bool XMLLoader::LoadWorld(World& world, const std::string& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* root = doc.RootElement();
		const XMLElement* sprite = root->FirstChildElement(XML_NODE_SPRITE);
		const XMLElement* collisionData = root->FirstChildElement(XML_NODE_COLLISION_DATA);

		const std::string& pathToSprite = sprite->Attribute(XML_PATH_ATTR);
		const std::string& pathToCollisionData = collisionData->Attribute(XML_PATH_ATTR);
		const std::string& bodyName = collisionData->Attribute(XML_NAME_ATTR);

		world.Init(pathToSprite, bodyName, pathToCollisionData);
	}

	return isSuccessful;
}

void XMLLoader::CreatePhysicsBodyFromAttributes(Node& attachmentNode, const XMLNode* xmlNode,
	Size& outSize)
{
	const XMLElement* physicsBodyElem =
		xmlNode->FirstChildElement(XML_NODE_PHYSICS_BODY);

	const std::string bodyType = physicsBodyElem->Attribute(XML_SHAPE_ATTR);
	const Size& bodySize = Size(physicsBodyElem->FloatAttribute(XML_WIDTH_ATTR),
		physicsBodyElem->FloatAttribute(XML_HEIGHT_ATTR));

	const bool isGravityEnabled = physicsBodyElem->BoolAttribute(XML_PHYSICS_GRAVITY_ATTR);
	const bool isBodyDynamic = physicsBodyElem->BoolAttribute(XML_PHYSICS_DYNAMIC_BODY_ATTR);
	const bool isRotationEnabled = physicsBodyElem->BoolAttribute(XML_PHYSICS_ROTATION_ENABLED_ATTR);
	const int collisionBitMask = physicsBodyElem->IntAttribute(XML_PHYSICS_BIT_MASK_ATTR);

	if (bodyType == XML_PHYSICS_BODY_BOX_ATTR)
	{
		const PhysicsMaterial& material = LoadPhysicsMaterialFromAttributes(xmlNode);
		PhysicsBodyConfig bodyConfig = PhysicsBodyConfig(
			bodySize,
			material,
			BodyType::Box,
			collisionBitMask,
			isBodyDynamic,
			isGravityEnabled);
		bodyConfig.SetRotationEnabled(isRotationEnabled);

		PhysicsManager::AddPhysicsBody(attachmentNode, bodyConfig);
	}

	outSize = bodySize;
}

GameInputType XMLLoader::StrToGameInputType(const std::string& inputType)
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
		Utils::AssertWithStrFormat(false,
			"XMLLoader: [strToGameInpuType] invalid input type: %s",
			inputType);
	}
	return type;
}

PhysicsMaterial XMLLoader::LoadPhysicsMaterialFromAttributes(
	const tinyxml2::XMLNode* pNode)
{
	const tinyxml2::XMLElement* pPhysicsMaterialElem = pNode->
		FirstChildElement(XML_NODE_PHYSICS_MATERIAL);
	const float density = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_DESITY_ATTR);
	const float restitution = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_RESTITUTION_ATTR);
	const float friction = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_FRICTION_ATTR);

	return { density, restitution, friction };
}

void XMLLoader::LoadVec3FromAttributes(const XMLElement* element, Vec3& outResult)
{
	outResult.x = element->ToElement()->FloatAttribute(XML_AXIS_X_ATTR);
	outResult.y = element->ToElement()->FloatAttribute(XML_AXIS_Y_ATTR);
	outResult.z = element->ToElement()->FloatAttribute(XML_AXIS_Z_ATTR);
}

void XMLLoader::LoadVec2FromAttributes(const XMLElement* element, Vec2& outResult)
{
	outResult.x = element->ToElement()->FloatAttribute(XML_AXIS_X_ATTR);
	outResult.y = element->ToElement()->FloatAttribute(XML_AXIS_Y_ATTR);
}
