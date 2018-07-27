#include "XMLLoader.h"
#include "physics/CCPhysicsBody.h"
#include "XMLConsts.h"
#include "World/Entity/Player/Player.h"
#include "../Utils.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Entity/Components/MirrorSpriteComponent.h"
#include "Input/GameInput.h"
#include "World/World.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "World/Entity/Components/Attack/RangedAttackComponent.h"
#include "World/Entity/Components/Attack/LongSwordAttackComponent.h"
#include "World/Entity/Components/Attack/HitAttackComponent.h"
#include "World/Physics/PhysicsBodyConfig.h"
#include "World/Physics/PhysicsManager.h"
#include "UI/InGameIndicators/ProgressBar.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "UI/UIElementConfig.h"
#include "Camera/Camera.h"
#include "Camera/Components/CameraShake.h"
#include "World/Entity/AI/SpawnPointConfig.h"
#include "World/Projectiles/ProjectileConfig.h"

NS_LIGHTSOULS_BEGIN

XMLLoader::XMLLoader()
{
	// Private constructor to prevent instance creation
}

bool XMLLoader::LoadXMLFile(const String& pathToXML, XMLDoc& outDoc)
{
	XMLError error = outDoc.LoadFile(pathToXML.c_str());

	if (error != XMLError::XML_NO_ERROR)
	{
		Utils::AssertWithStrFormat(false, "XMLLoader: Failed to load file from path %s",
			pathToXML);
	}

	return !error;
}

bool XMLLoader::InitializeAIManagerUsingXMLFile(AIAgentManager& aiManager, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* pData = doc.RootElement();
		for (XMLElement* node = pData->FirstChildElement(); node;
			node = node->NextSiblingElement())
		{
			const String& elementName = node->Value();
			if (elementName == XML_NODE_AGENT_CONFIG_LIST)
			{
				// Load all agent configurations
				for (XMLElement* child = node->FirstChildElement(); child != nullptr;
					child = child->NextSiblingElement())
				{
					const String& path = child->Attribute(XML_PATH_ATTR);
					const String& type = child->Attribute(XML_TYPE_ATTR);
					aiManager.AddAgentConfig(type, path);
				}
			}
			else if (elementName == XML_NODE_SPAWN_POINTS)
			{
				// Load all agent spawn points
				for (XMLElement* child = node->FirstChildElement(); child != nullptr;
					child = child->NextSiblingElement())
				{
					Vector2 position;
					GetVector2FromElement(child, position);
					String agentType = child->Attribute(XML_TYPE_ATTR);
					int spawnCount = child->IntAttribute(XML_AI_SPAWN_COUNT);
					int numAgentsInRow = child->IntAttribute(XML_AI_NUM_AGENTS_IN_ROW);
					float spawnDelay = child->FloatAttribute(XML_AI_SPAWN_DELAY);
					float rowPadding = child->FloatAttribute(XML_AI_ROW_PADDING);
					float columnPadding = child->FloatAttribute(XML_AI_COLUMN_PADDING);

					SpawnPointConfig config;
					config.SetAgentType(agentType);
					config.SetPosition(position);
					config.SetSpawnCount(spawnCount);
					config.SetNumAgentsInRow(numAgentsInRow);
					config.SetSpawnDelay(spawnDelay);
					config.SetAgentRowPadding(rowPadding);
					config.SetAgentColumnPadding(columnPadding);

					aiManager.AddSpawnPoint(config);
				}
			}
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeProjectileConfig(ProjectileConfig& config, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* data = doc.RootElement();

		const float moveSpeed = data->FloatAttribute(XML_MOVE_SPEED_ATTR);
		config.SetMoveSpeed(moveSpeed);

		// Loop components
		for (XMLElement* element = data->FirstChildElement(); element;
			element = element->NextSiblingElement())
		{
			const String& componentType = element->Attribute(XML_TYPE_ATTR);
			if (componentType == RIGID_BODY_COMPONENT)
			{
				PhysicsBodyConfig rigidBodyConfig;
				CreatePhysicsBodyFromAttributes(element, rigidBodyConfig);
				config.SetPhysicsBodyConfig(rigidBodyConfig);
			}
			else if (componentType == SPRITE_COMPONENT)
			{
				const String& pathToSprite = element->Attribute(XML_PATH_ATTR);
				config.SetPathToSprite(pathToSprite);
			}
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeCamera(Camera& camera, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* data = doc.RootElement();
		for (XMLElement* element = data->FirstChildElement(); element;
			element = element->NextSiblingElement())
		{
			const String& type = element->Attribute(XML_TYPE_ATTR);

			if (type == CAMERA_SHAKE_COMPONENT)
			{
				auto cameraShake = CameraShake::Create(element);
				camera.addComponent(cameraShake);
			}
			else if (type == CAMERA_FOLLOW_COMPONENT)
			{
				//camera.addComponent(CameraFollow::Create());
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
		const String& childNodeName = pChild->Value();
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
	const String& actionName)
{
	// Go trough all keys that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const String& keyCodeFrom = pChild->Attribute(XML_INPUT_KEY_FROM_ATTR);
		const String& keyCodeTo = pChild->Attribute(XML_INPUT_KEY_TO_ATTR);

		const float valueFrom = pChild->FloatAttribute(XML_INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_INPUT_VALUE_TO_ATTR);

		gameInput.AddAxisActionInput(GameInputType::KEYBOARD,
			actionName, keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	}
}

void XMLLoader::LoadGameControllerAxis(GameInput& gameInput, const XMLElement* pElement, const String& actionName)
{
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const String& axisName = pChild->Attribute(XML_INPUT_AXIS_NAME);
		const float valueFrom = pChild->FloatAttribute(XML_INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XML_INPUT_VALUE_TO_ATTR);

		gameInput.AddAxisActionInput(GameInputType::GAME_CONTROLLER,
			actionName, axisName, axisName, valueFrom, valueTo);
	}
}

void XMLLoader::LoadActionButton(GameInput& gameInput, GameInputType inputType, const XMLElement* pElement,
	const String& actionName, const String& xmlAttributeName)
{
	// Go trough all buttons that involve this action
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const char* buttonCode = pChild->Attribute(xmlAttributeName.c_str());
		if (buttonCode != nullptr)
		{
			gameInput.AddActionInput(inputType, actionName, buttonCode);
		}
		else
		{
			CCASSERT(false, "XMLLoader: [LoadActionButton] missing button code !");
		}
	}
}

void XMLLoader::LoadUIElement(const XMLElement* element, UIElementConfig& outUIElement)
{
	const XMLElement* spriteNode = element->FirstChildElement(XML_NODE_SPRITE);
	const XMLElement* anchorPositionNode = element->FirstChildElement(XML_NODE_NORMALIZED_POSITION);
	const XMLElement* normalizedPositionNode = element->FirstChildElement(XML_NODE_NORMALIZED_POSITION);
	const XMLElement* scaleNode = element->FirstChildElement(XML_NODE_SCALE);

	const String& pathToSprite = spriteNode->Attribute(XML_PATH_ATTR);

	Vector2 anchorPos;
	GetVector2FromElement(anchorPositionNode, anchorPos);

	Vector2 normalizedPos;
	GetVector2FromElement(normalizedPositionNode, normalizedPos);

	Vector2 scale;
	GetVector2FromElement(scaleNode, scale);

	outUIElement.SetPathToSprite(pathToSprite);
	outUIElement.SetAnchorPosition(anchorPos);
	outUIElement.SetNormalizedPosition(normalizedPos);
	outUIElement.SetScale(scale);
}

bool XMLLoader::InitializeEntityUsingXMLFile(Entity& entity,
	const String& pathToXML)
{
	// Load the file
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);

	if (isSuccessful)
	{
		XMLElement* root = doc.RootElement();

		// Load root entity attributes
		const String actorType = root->Attribute(XML_TYPE_ATTR);
		const float moveSpeed = root->FloatAttribute(XML_MOVE_SPEED_ATTR);
		const float baseHealth = root->FloatAttribute(XML_ENTITY_BASE_HEALTH_ATTR);
		const float baseStamina = root->FloatAttribute(XML_ENTITY_BASE_STAMINA_ATTR);
		const float baseDamage = root->FloatAttribute(XML_ENTITY_BASE_DAMAGE_ATTR);
		const float staminaRegenerateSpeed = root->FloatAttribute(XML_ENTITY_STAMINA_REGENARATE_SPEED_ATTR);
		const float staminaRegenerateDelay = root->FloatAttribute(XML_ENTITY_STAMINA_REGENARATE_DELAY_ATTR);
		const float	knockBackStrenght = root->FloatAttribute(XML_ENTITY_KNOCK_BACK_STRENGHT);

		entity.setName(actorType);
		entity.SetBaseMoveSpeed(moveSpeed);
		entity.SetBaseDamage(baseDamage);
		entity.SetBaseHealth(baseHealth);
		entity.SetBaseStamina(baseStamina);
		entity.SetStaminaRegenerateSpeed(staminaRegenerateSpeed);
		entity.SetStaminaRegenerateDelay(staminaRegenerateDelay);
		entity.SetKnockBackStrenght(knockBackStrenght);

		// Load entity components
		for (XMLElement* element = root->FirstChildElement(); element;
			element = element->NextSiblingElement())
		{
			// Check all actor components
			const String componentType = element->Attribute(XML_TYPE_ATTR);

			if (componentType == TRANSFORM_COMPONENT)
			{
				// Transform component has data types in specific order
				XMLElement* positionNode = element->FirstChildElement(XML_NODE_POSITION);
				XMLElement* rotationNode = element->FirstChildElement(XML_NODE_ROTATION);
				XMLElement* scaleNode = element->FirstChildElement(XML_NODE_SCALE);

				Vector3 position;
				GetVector3FromElement(positionNode, position);

				Vector3 rotation;
				GetVector3FromElement(rotationNode, rotation);

				Vector3 scale;
				GetVector3FromElement(scaleNode, scale);

				entity.setPosition3D(position);
				entity.setRotation3D(rotation);
				entity.setScale(scale.x, scale.y);
			}
			else if (componentType == PLAYER_CONTROLLER_COMPONENT)
			{
				const float dodgeSpeed = element->FloatAttribute(XML_PLAYER_DODGE_SPEED_ATTR);
				const float dodgeTime = element->FloatAttribute(XML_PLAYER_DODGE_TIME_ATTR);
				const float dodgeStaminaConsumption = element->FloatAttribute(XML_PLAYER_DODGE_STAMINA_ATTR);

				Player* player = dynamic_cast<Player*>(&entity);
				player->SetDodgeSpeed(dodgeSpeed);
				player->SetDodgeTime(dodgeTime);
				player->SetDodgeStaminaConsumption(dodgeStaminaConsumption);
			}
			else if (componentType == AI_CONTROLLER_COMPONENT)
			{
				AIAgent* agent = static_cast<AIAgent*>(&entity);
				if (agent != nullptr)
				{
					const String& agentType = root->Attribute(XML_TYPE_ATTR);
					agent->SetAgentType(agentType);
					agent->Init(element);
				}
				else
				{
					CCASSERT(false, "Trying to add AIController component to entity that is not of type AIAgent !");
				}
			}
			else if (componentType == ANIM_COMPONENT)
			{
				AnimComponent* animComponent = AnimComponent::Create(entity);
				animComponent->setName(ANIM_COMPONENT);
				animComponent->LoadConfig(element);
				entity.addComponent(animComponent);
			}
			else if (componentType == RANGED_ATTACK_COMPONENT)
			{
				const float range = element->FloatAttribute(XML_ENTITY_ATTACK_RANGE_ATTR);
				const float secondsBetweenAttacks = element->FloatAttribute(XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);

				XMLElement* projectileElem = element->FirstChildElement(XML_NODE_PROJECTILE);
				const String& pathToProjectile = projectileElem->Attribute(XML_PATH_ATTR);
				ProjectileConfig config;
				config.Init(pathToProjectile);

				RangedAttackComponent* rangedAttack = RangedAttackComponent::Create(config, range, secondsBetweenAttacks);
				rangedAttack->setName(ATTACK_COMPONENT);
				entity.addComponent(rangedAttack);
			}
			else if (componentType == GENERIC_ATTACK_COMPONENT)
			{
				const float secondsBetweenAttacks = element->FloatAttribute(XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
				const float attackRange = element->FloatAttribute(XML_ENTITY_ATTACK_RANGE_ATTR);
				const float staminaConsumption = element->FloatAttribute(XML_ATTACK_STAMINA_CONSUMPTION_ATTR);
				const float comboExpireTime = element->FloatAttribute(XML_ENTITY_COMBO_EXPIRE_TIME_ATTR);

				auto attackComponent = GenericAttackComponent::Create(secondsBetweenAttacks, attackRange);
				attackComponent->SetStaminaConsumption(staminaConsumption);
				attackComponent->SetComboExpireTime(comboExpireTime);
				attackComponent->setName(ATTACK_COMPONENT);
				entity.addComponent(attackComponent);
			}
			else if (componentType == LONG_SWORD_ATTACK_COMPONENT)
			{
				const float secondsBetweenAttacks = element->FloatAttribute(XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
				const float attackRange = element->FloatAttribute(XML_ENTITY_ATTACK_RANGE_ATTR);
				const float paddingFromBody = element->FloatAttribute(XML_ENTITY_PADDING_FROM_BODY_ATTR);
				const float staminaConsumption = element->FloatAttribute(XML_ATTACK_STAMINA_CONSUMPTION_ATTR);
				const float comboExpireTime = element->FloatAttribute(XML_ENTITY_COMBO_EXPIRE_TIME_ATTR);

				LongSwordAttackComponent* longSwordAttack =
					LongSwordAttackComponent::Create(secondsBetweenAttacks, attackRange,
						paddingFromBody);

				longSwordAttack->setName(ATTACK_COMPONENT);
				longSwordAttack->SetStaminaConsumption(staminaConsumption);
				longSwordAttack->SetComboExpireTime(comboExpireTime);
				entity.addComponent(longSwordAttack);
			}
			else if (componentType == HIT_ATTACK_COMPONENT)
			{
				const float secondsBetweenAttacks = element->FloatAttribute(XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
				const float attackRange = element->FloatAttribute(XML_ENTITY_ATTACK_RANGE_ATTR);
				const float staminaConsumption = element->FloatAttribute(XML_ATTACK_STAMINA_CONSUMPTION_ATTR);
				const float comboExpireTime = element->FloatAttribute(XML_ENTITY_COMBO_EXPIRE_TIME_ATTR);

				HitAttackComponent* hitAttack = HitAttackComponent::Create(secondsBetweenAttacks, attackRange);
				hitAttack->setName(ATTACK_COMPONENT);
				hitAttack->SetStaminaConsumption(staminaConsumption);
				hitAttack->SetComboExpireTime(comboExpireTime);
				entity.addComponent(hitAttack);
			}
			else if (componentType == RIGID_BODY_COMPONENT)
			{
				PhysicsBodyConfig config;
				CreatePhysicsBodyFromAttributes(element, config);
				PhysicsManager::AddPhysicsBody(entity, config);

				const XMLElement* physicsBodyElem = element->FirstChildElement(XML_NODE_PHYSICS_BODY);
				const XMLElement* bodyAnchorElem = element->FirstChildElement(XML_NODE_ANCHOR_POSITION);

				const float forceScale = physicsBodyElem->FloatAttribute(XML_PHYSICS_FORCE_SCALE_ATTR);
				Vector2 anchorPosition;
				GetVector2FromElement(bodyAnchorElem, anchorPosition);

				entity.SetPhysicsBodyForceScale(forceScale);
				entity.SetPhysicsBodySize(config.GetSize());
				entity.SetPhysicsBodyAnchor(anchorPosition);
			}
			else if (componentType == MIRROR_SPRITE_COMPONENT)
			{
				MirrorSpriteComponent* pMirrorSprite = MirrorSpriteComponent::create();
				pMirrorSprite->setName(MIRROR_SPRITE_COMPONENT);
				pMirrorSprite->setOwnerEntity(dynamic_cast<Entity*>(&entity));
				entity.addComponent(pMirrorSprite);
			}
			else if (componentType == NODE_COMPONENT)
			{
				cocos2d::Node* node = cocos2d::Node::create();
				node->setName(entity.getName() + NODE_COMPONENT);
				node->setContentSize(entity.getContentSize());
				node->setAnchorPoint(Vector2(0, 0));
				LoadNodeComponents(node, element);
				entity.addChild(node);
			}
		}
	}
	return isSuccessful;
}

bool XMLLoader::LoadInputSettings(GameInput& gameInput, const String& pathToConfigXml)
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
			const String& nodeName = pElem->Value();
			if (nodeName == XML_INPUT_AXIS)
			{
				LoadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadKeyboardAxis(gameInput, pElem, actionName);
				},

					nullptr, // Currently not handling Mouse axis input

					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadGameControllerAxis(gameInput, pElem, actionName);
				});
			}
			else if (nodeName == XML_INPUT_ACTION_BUTTON)
			{
				LoadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadActionButton(gameInput, GameInputType::KEYBOARD, pElem,
						actionName, XML_INPUT_KEY_CODE_ATTR);
				},
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadActionButton(gameInput, GameInputType::MOUSE, pElem,
						actionName, XML_INPUT_BUTTON_CODE_ATTR);
				},
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadActionButton(gameInput, GameInputType::GAME_CONTROLLER, pElem,
						actionName, XML_INPUT_BUTTON_CODE_ATTR);
				});
			}
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeUIProgressBar(ProgressBar& healthBar, const String& pathToXML)
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

bool XMLLoader::LoadWorld(World& world, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* root = doc.RootElement();
		const XMLElement* sprite = root->FirstChildElement(XML_NODE_SPRITE);
		const XMLElement* collisionData = root->FirstChildElement(XML_NODE_COLLISION_DATA);

		const String& pathToSprite = sprite->Attribute(XML_PATH_ATTR);
		const String& pathToCollisionData = collisionData->Attribute(XML_PATH_ATTR);
		const String& bodyName = collisionData->Attribute(XML_NAME_ATTR);

		world.Init(pathToSprite, bodyName, pathToCollisionData);
	}

	return isSuccessful;
}

void XMLLoader::CreatePhysicsBodyFromAttributes(const XMLNode* xmlNode, PhysicsBodyConfig& outConfig)
{
	const XMLElement* physicsBodyElem = xmlNode->FirstChildElement(XML_NODE_PHYSICS_BODY);
	const String bodyType = physicsBodyElem->Attribute(XML_SHAPE_ATTR);

	const float width = physicsBodyElem->FloatAttribute(XML_WIDTH_ATTR);
	const float height = physicsBodyElem->FloatAttribute(XML_HEIGHT_ATTR);
	const cocos2d::Size& bodySize = cocos2d::Size(width, height);

	const bool isGravityEnabled = physicsBodyElem->BoolAttribute(XML_PHYSICS_GRAVITY_ATTR);
	const bool isBodyDynamic = physicsBodyElem->BoolAttribute(XML_PHYSICS_DYNAMIC_BODY_ATTR);
	const bool isRotationEnabled = physicsBodyElem->BoolAttribute(XML_PHYSICS_ROTATION_ENABLED_ATTR);
	const int collisionBitMask = physicsBodyElem->IntAttribute(XML_PHYSICS_BIT_MASK_ATTR);

	if (bodyType == XML_PHYSICS_BODY_BOX_ATTR)
	{
		cocos2d::PhysicsMaterial material;
		LoadPhysicsMaterialFromAttributes(xmlNode, material);

		outConfig.SetSize(bodySize);
		outConfig.SetPhysicsMaterial(material);
		outConfig.SetBodyType(BodyType::BOX);
		outConfig.SetCollisionBitMask(collisionBitMask);
		outConfig.SetIsDynamic(isBodyDynamic);
		outConfig.SetIsGravityEnabled(isGravityEnabled);
		outConfig.SetRotationEnabled(isRotationEnabled);
	}
}

void XMLLoader::LoadNodeComponents(cocos2d::Node* node, const XMLElement* root)
{
	for (const XMLElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		const String& componentType = element->Attribute(XML_TYPE_ATTR);
		if (componentType == RIGID_BODY_COMPONENT)
		{
			PhysicsBodyConfig config;
			CreatePhysicsBodyFromAttributes(element, config);
			PhysicsManager::AddPhysicsBody(*node, config);
		}
	}
}

GameInputType XMLLoader::StrToGameInputType(const String& inputType)
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

void XMLLoader::ReadXMLAttribute(const XMLElement* element, const String& attributeName, String& outValue)
{
	const char* value = element->Attribute(attributeName.c_str());
	if (value != nullptr)
	{
		outValue = value;
	}
}

void XMLLoader::LoadPhysicsMaterialFromAttributes(const tinyxml2::XMLNode* pNode, cocos2d::PhysicsMaterial& outMaterial)
{
	const tinyxml2::XMLElement* pPhysicsMaterialElem = pNode->
		FirstChildElement(XML_NODE_PHYSICS_MATERIAL);
	const float density = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_DESITY_ATTR);
	const float restitution = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_RESTITUTION_ATTR);
	const float friction = pPhysicsMaterialElem->FloatAttribute(XML_PHYSICS_FRICTION_ATTR);

	outMaterial.density = density;
	outMaterial.restitution = restitution;
	outMaterial.friction = friction;
}

void XMLLoader::GetVector3FromElement(const XMLElement* element, Vector3& outResult)
{
	outResult.x = element->ToElement()->FloatAttribute(XML_AXIS_X_ATTR);
	outResult.y = element->ToElement()->FloatAttribute(XML_AXIS_Y_ATTR);
	outResult.z = element->ToElement()->FloatAttribute(XML_AXIS_Z_ATTR);
}

void XMLLoader::GetVector2FromElement(const XMLElement* element, Vector2& outResult)
{
	outResult.x = element->ToElement()->FloatAttribute(XML_AXIS_X_ATTR);
	outResult.y = element->ToElement()->FloatAttribute(XML_AXIS_Y_ATTR);
}

NS_LIGHTSOULS_END
