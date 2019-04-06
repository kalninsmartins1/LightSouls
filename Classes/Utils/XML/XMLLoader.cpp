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
#include "World/Entity/CustomActions/AI/AIAvoidTargetAction.h"
#include "World/Entity/Components/LookAtAITargetComponent.h"
#include "World/GameSpeedModifier.h"
#include "World/VFX/VFXManager.h"

XMLLoader::XMLLoader()
{
	// Private constructor to prevent instance creation
}

bool XMLLoader::GetAnchorPosition(const XMLElement& element, Vector2& outPos)
{
	const XMLElement* bodyAnchorElem = element.FirstChildElement(XMLConsts::NODE_ANCHOR_POSITION);
	bool hasResult = false;

	if (bodyAnchorElem != nullptr)
	{		
		GetVector2FromElement(*bodyAnchorElem, outPos);
		hasResult = true;
	}

	return hasResult;
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
			if (elementName == XMLConsts::NODE_AGENT_CONFIG_LIST)
			{
				// Load all agent configurations
				for (XMLElement* child = node->FirstChildElement(); child != nullptr;
					child = child->NextSiblingElement())
				{
					const String& path = child->Attribute(XMLConsts::PATH_ATTR);
					const String& type = child->Attribute(XMLConsts::TYPE_ATTR);
					aiManager.AddAgentConfig(type, path);
				}
			}
			else if (elementName == XMLConsts::NODE_SPAWN_POINTS)
			{
				// Load all agent spawn points
				for (XMLElement* child = node->FirstChildElement(); child != nullptr;
					child = child->NextSiblingElement())
				{
					Vector2 position;
					GetVector2FromElement(*child, position);
					String agentType = child->Attribute(XMLConsts::TYPE_ATTR);
					int spawnCount = child->IntAttribute(XMLConsts::AI_SPAWN_COUNT);
					int numAgentsInRow = child->IntAttribute(XMLConsts::AI_NUM_AGENTS_IN_ROW);
					float spawnDelay = child->FloatAttribute(XMLConsts::AI_SPAWN_DELAY);
					float rowPadding = child->FloatAttribute(XMLConsts::AI_ROW_PADDING);
					float columnPadding = child->FloatAttribute(XMLConsts::AI_COLUMN_PADDING);

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

		const float moveSpeed = data->FloatAttribute(XMLConsts::MOVE_SPEED_ATTR);
		config.SetMoveSpeed(moveSpeed);

		// Loop components
		for (XMLElement* element = data->FirstChildElement(); element != nullptr;
			element = element->NextSiblingElement())
		{
			const String& componentType = element->Attribute(XMLConsts::TYPE_ATTR);
			if (componentType == GameConsts::RIGID_BODY_COMPONENT)
			{
				PhysicsBodyConfig rigidBodyConfig;
				CreatePhysicsBodyFromAttributes(*element, rigidBodyConfig);
				config.SetPhysicsBodyConfig(rigidBodyConfig);
			}
			else if (componentType == GameConsts::SPRITE_COMPONENT)
			{
				const String& pathToSprite = element->Attribute(XMLConsts::PATH_ATTR);
				config.SetPathToSprite(pathToSprite);
			}
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeComponents(cc::Node& node, const String& pathToXML)
{
	XMLDoc doc;
	bool isSuccessful = LoadXMLFile(pathToXML, doc);

	if (isSuccessful)
	{
		XMLElement* root = doc.RootElement();

		for (XMLElement* element = root->FirstChildElement(); element != nullptr;
			element = element->NextSiblingElement())
		{
			// Check all actor components
			const String componentType = element->Attribute(XMLConsts::TYPE_ATTR);
			isSuccessful &= InitializeComponent(node, *element, componentType);
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeComponent(cc::Node& node, const XMLElement& element, const String& componentType)
{
	bool isSuccessful = true;

	if (componentType == GameConsts::TRANSFORM_COMPONENT)
	{
		// Transform component has data types in specific order
		const XMLElement* positionNode = element.FirstChildElement(XMLConsts::NODE_POSITION);
		const XMLElement* rotationNode = element.FirstChildElement(XMLConsts::NODE_ROTATION);
		const XMLElement* scaleNode = element.FirstChildElement(XMLConsts::NODE_SCALE);

		if (positionNode != nullptr && rotationNode != nullptr && scaleNode != nullptr)
		{
			Vector3 position;
			GetVector3FromElement(*positionNode, position);

			Vector3 rotation;
			GetVector3FromElement(*rotationNode, rotation);

			Vector3 scale;
			GetVector3FromElement(*scaleNode, scale);

			node.setPosition3D(position);
			node.setRotation3D(rotation);
			node.setScale(scale.x, scale.y);
		}
		else
		{
			isSuccessful = false;
		}
	}
	else if (componentType == GameConsts::PLAYER_CONTROLLER_COMPONENT)
	{
		const float dodgeSpeed = element.FloatAttribute(XMLConsts::PLAYER_DODGE_SPEED_ATTR);
		const float dodgeTime = element.FloatAttribute(XMLConsts::PLAYER_DODGE_TIME_ATTR);
		const float dodgeStaminaConsumption = element.FloatAttribute(XMLConsts::PLAYER_DODGE_STAMINA_ATTR);

		Player* player = static_cast<Player*>(&node);
		if (player != nullptr)
		{
			player->SetDodgeSpeed(dodgeSpeed);
			player->SetDodgeTime(dodgeTime);
			player->SetDodgeStaminaConsumption(dodgeStaminaConsumption);
		}
		else
		{
			isSuccessful = false;
		}
	}
	else if (componentType == GameConsts::AI_CONTROLLER_COMPONENT)
	{
		AIAgent* agent = static_cast<AIAgent*>(&node);
		const XMLElement* root = element.Parent()? element.Parent()->ToElement() : nullptr;
		if (agent != nullptr && root != nullptr)
		{
			const String& agentType = root->Attribute(XMLConsts::TYPE_ATTR);
			agent->SetAgentType(agentType);
			agent->Init(&element);
		}
		else
		{
			isSuccessful = false;
			CCASSERT(false, "Trying to add AIController component to entity that is not of type AIAgent !");
		}
	}
	else if (componentType == GameConsts::BACKGROUND_ACTION_COMPONENT)
	{
		LoadBackgroundActions(node, element);
	}
	else if (componentType == GameConsts::ANIM_COMPONENT)
	{
		AnimComponent* animComponent = AnimComponent::Create(node);
		animComponent->setName(GameConsts::ANIM_COMPONENT);
		animComponent->LoadConfig(element);
		node.addComponent(animComponent);
	}
	else if (componentType == GameConsts::RANGED_ATTACK_COMPONENT)
	{
		const float range = element.FloatAttribute(XMLConsts::ENTITY_ATTACK_RANGE_ATTR);
		const float secondsBetweenAttacks = element.FloatAttribute(XMLConsts::ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);

		const XMLElement* projectileElem = element.FirstChildElement(XMLConsts::NODE_PROJECTILE);
		if (projectileElem != nullptr)
		{
			const String& pathToProjectile = projectileElem->Attribute(XMLConsts::PATH_ATTR);
			ProjectileConfig config;
			config.Init(pathToProjectile);

			RangedAttackComponent* rangedAttack = RangedAttackComponent::Create(config, range, secondsBetweenAttacks);
			rangedAttack->setName(GameConsts::ATTACK_COMPONENT);
			node.addComponent(rangedAttack);
		}
		else
		{
			isSuccessful = false;
		}
	}
	else if (componentType == GameConsts::GENERIC_ATTACK_COMPONENT)
	{
		const float secondsBetweenAttacks = element.FloatAttribute(XMLConsts::ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
		const float attackRange = element.FloatAttribute(XMLConsts::ENTITY_ATTACK_RANGE_ATTR);
		const float staminaConsumption = element.FloatAttribute(XMLConsts::ATTACK_STAMINA_CONSUMPTION_ATTR);
		const float comboExpireTime = element.FloatAttribute(XMLConsts::ENTITY_COMBO_EXPIRE_TIME_ATTR);

		auto attackComponent = GenericAttackComponent::Create(secondsBetweenAttacks, attackRange);
		attackComponent->SetComboExpireTime(comboExpireTime);
		attackComponent->setName(GameConsts::ATTACK_COMPONENT);
		node.addComponent(attackComponent);
	}
	else if (componentType == GameConsts::LONG_SWORD_ATTACK_COMPONENT)
	{
		const float secondsBetweenAttacks = element.FloatAttribute(XMLConsts::ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
		const float attackRange = element.FloatAttribute(XMLConsts::ENTITY_ATTACK_RANGE_ATTR);
		const float paddingFromBody = element.FloatAttribute(XMLConsts::ENTITY_PADDING_FROM_BODY_ATTR);
		const float staminaConsumption = element.FloatAttribute(XMLConsts::ATTACK_STAMINA_CONSUMPTION_ATTR);
		const float comboExpireTime = element.FloatAttribute(XMLConsts::ENTITY_COMBO_EXPIRE_TIME_ATTR);

		LongSwordAttackComponent* longSwordAttack =
			LongSwordAttackComponent::Create(secondsBetweenAttacks, attackRange,
				paddingFromBody);

		longSwordAttack->setName(GameConsts::ATTACK_COMPONENT);
		longSwordAttack->SetComboExpireTime(comboExpireTime);
		node.addComponent(longSwordAttack);
	}
	else if (componentType == GameConsts::AI_LOOK_AT_TARGET_COMPONENT)
	{
		LookAtAITargetComponent* lookAtComponent = LookAtAITargetComponent::create();
		lookAtComponent->SetOwnerEntity(node);
		node.addComponent(lookAtComponent);
	}
	else if (componentType == GameConsts::HIT_ATTACK_COMPONENT)
	{
		const float secondsBetweenAttacks = element.FloatAttribute(XMLConsts::ENTITY_SECONDS_BETWEEN_ATTACK_ATTR);
		const float attackRange = element.FloatAttribute(XMLConsts::ENTITY_ATTACK_RANGE_ATTR);
		const float staminaConsumption = element.FloatAttribute(XMLConsts::ATTACK_STAMINA_CONSUMPTION_ATTR);
		const float comboExpireTime = element.FloatAttribute(XMLConsts::ENTITY_COMBO_EXPIRE_TIME_ATTR);

		HitAttackComponent* hitAttack = HitAttackComponent::Create(secondsBetweenAttacks, attackRange);
		hitAttack->setName(GameConsts::ATTACK_COMPONENT);
		hitAttack->SetComboExpireTime(comboExpireTime);
		node.addComponent(hitAttack);
	}
	else if (componentType == GameConsts::RIGID_BODY_COMPONENT)
	{
		PhysicsBodyConfig config;
		CreatePhysicsBodyFromAttributes(element, config);
		PhysicsManager::AddPhysicsBody(node, config);

		const XMLElement* physicsBodyElem = element.FirstChildElement(XMLConsts::NODE_PHYSICS_BODY);
		const XMLElement* bodyAnchorElem = element.FirstChildElement(XMLConsts::NODE_ANCHOR_POSITION);		

		Entity* entity = static_cast<Entity*>(&node);
		if (entity != nullptr && 
			physicsBodyElem != nullptr &&
			bodyAnchorElem != nullptr)
		{
			const float forceScale = physicsBodyElem->FloatAttribute(XMLConsts::PHYSICS_FORCE_SCALE_ATTR);
			Vector2 anchorPosition;
			GetVector2FromElement(*bodyAnchorElem, anchorPosition);

			entity->SetPhysicsBodyForceScale(forceScale);
			entity->SetPhysicsBodySize(config.GetSize());
			entity->SetPhysicsBodyAnchor(anchorPosition);
		}
		else
		{
			isSuccessful = false;
		}
	}
	else if (componentType == GameConsts::MIRROR_SPRITE_COMPONENT)
	{
		float sensitivity = element.FloatAttribute(XMLConsts::SENSITIVITY);
		MirrorSpriteComponent* mirrorSprite = MirrorSpriteComponent::create();
		mirrorSprite->setName(GameConsts::MIRROR_SPRITE_COMPONENT);
		mirrorSprite->Init(node, sensitivity);
		node.addComponent(mirrorSprite);
	}
	else if (componentType == GameConsts::NODE_COMPONENT)
	{
		cc::Node* childNode = cc::Node::create();
		if (childNode != nullptr)
		{
			childNode->setName(node.getName() + GameConsts::NODE_COMPONENT);
			childNode->setContentSize(node.getContentSize());
			childNode->setAnchorPoint(Vector2(0, 0));
			LoadNodeComponents(*childNode, element);
			node.addChild(childNode);
		}
		else
		{
			isSuccessful = false;
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeGameSpeedModifier(GameSpeedModifier& speedModifier, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* data = doc.RootElement();
		for (XMLElement* element = data->FirstChildElement(); element != nullptr;
			element = element->NextSiblingElement())
		{
			const String& eventType = element->Attribute(XMLConsts::TYPE_ATTR);
			const float speed = element->FloatAttribute(XMLConsts::SPEED_ATTR);
			const float duration = element->FloatAttribute(XMLConsts::DURATION_ATTR);
			speedModifier.AddEvent(eventType, speed, duration);	
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeVFXManager(VFXManager& vfxManager, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* data = doc.RootElement();
		for (XMLElement* element = data->FirstChildElement(); element != nullptr;
			element = element->NextSiblingElement())
		{
			const String& eventType = element->Attribute(XMLConsts::TYPE_ATTR);
			const String& pathToVfx = element->Attribute(XMLConsts::PATH_ATTR);
			vfxManager.AddVFX(eventType, pathToVfx);
		}
	}

	return isSuccessful;
}

bool XMLLoader::InitializeCamera(LS::Camera& camera, const String& pathToXML)
{
	XMLDoc doc;
	const bool isSuccessful = LoadXMLFile(pathToXML, doc);
	if (isSuccessful)
	{
		XMLElement* data = doc.RootElement();
		for (XMLElement* element = data->FirstChildElement(); element;
			element = element->NextSiblingElement())
		{
			const String& type = element->Attribute(XMLConsts::TYPE_ATTR);

			if (type == GameConsts::CAMERA_SHAKE_COMPONENT)
			{
				auto cameraShake = CameraShake::Create(element);
				camera.addComponent(cameraShake);
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
		Attribute(XMLConsts::NAME_ATTR);

	// Get element action triggers
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const String& childNodeName = pChild->Value();
		if (childNodeName == XMLConsts::INPUT_TYPE_KEYBOARD)
		{
			if (onKeyboardInput != nullptr)
			{
				onKeyboardInput(gameInput, pChild, actionName);
			}
		}
		else if (childNodeName == XMLConsts::INPUT_TYPE_MOUSE)
		{
			if (onMouseInput != nullptr)
			{
				onMouseInput(gameInput, pChild, actionName);
			}
		}
		else if (childNodeName == XMLConsts::INPUT_TYPE_GAME_CONTROLLER)
		{
			if (onGameControllerInput != nullptr)
			{
				onGameControllerInput(gameInput, pChild, actionName);
			}
		}
	}
}

void XMLLoader::LoadBackgroundActions(cc::Node& node, const XMLElement& xmlElement)
{
	for (auto element = xmlElement.FirstChildElement(); element != nullptr; 
		element = element->NextSiblingElement())
	{
		const String& type = element->Attribute(XMLConsts::TYPE_ATTR);		
		if (type == GameConsts::AVOID_TARGET_ACTION)
		{
			AIAgent* agent = static_cast<AIAgent*>(&node);
			CCASSERT(agent != nullptr, "Avoid action added to entity that is not AIAgent !");
			if (agent != nullptr)
			{
				auto avoidAction = AIAvoidTargetAction::Create(*agent);
				avoidAction->LoadXMLData(element);
				avoidAction->setTag(GameConsts::ACTION_AI_TASK_AVOID);
				node.runAction(avoidAction);
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
		const String& keyCodeFrom = pChild->Attribute(XMLConsts::INPUT_KEY_FROM_ATTR);
		const String& keyCodeTo = pChild->Attribute(XMLConsts::INPUT_KEY_TO_ATTR);

		const float valueFrom = pChild->FloatAttribute(XMLConsts::INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XMLConsts::INPUT_VALUE_TO_ATTR);

		gameInput.AddAxisActionInput(GameInputType::KEYBOARD,
			actionName, keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	}
}

void XMLLoader::LoadGameControllerAxis(GameInput& gameInput, const XMLElement* pElement, const String& actionName)
{
	for (const XMLElement* pChild = pElement->FirstChildElement();
		pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		const String& axisName = pChild->Attribute(XMLConsts::INPUT_AXIS_NAME);
		const float valueFrom = pChild->FloatAttribute(XMLConsts::INPUT_VALUE_FROM_ATTR);
		const float valueTo = pChild->FloatAttribute(XMLConsts::INPUT_VALUE_TO_ATTR);

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

void XMLLoader::LoadUIElement(const XMLElement& element, UIElementConfig& outUIElement)
{
	const XMLElement* spriteElement = element.FirstChildElement(XMLConsts::NODE_SPRITE);
	const XMLElement* anchorPositionElement = element.FirstChildElement(XMLConsts::NODE_NORMALIZED_POSITION);
	const XMLElement* normalizedPositionElement = element.FirstChildElement(XMLConsts::NODE_NORMALIZED_POSITION);
	const XMLElement* scaleElement = element.FirstChildElement(XMLConsts::NODE_SCALE);

	if (spriteElement != nullptr &&
		anchorPositionElement != nullptr &&
		normalizedPositionElement != nullptr &&
		scaleElement != nullptr)
	{
		const String& pathToSprite = spriteElement->Attribute(XMLConsts::PATH_ATTR);

		Vector2 anchorPos;
		GetVector2FromElement(*anchorPositionElement, anchorPos);

		Vector2 normalizedPos;
		GetVector2FromElement(*normalizedPositionElement, normalizedPos);

		Vector2 scale;
		GetVector2FromElement(*scaleElement, scale);

		outUIElement.SetPathToSprite(pathToSprite);
		outUIElement.SetAnchorPosition(anchorPos);
		outUIElement.SetNormalizedPosition(normalizedPos);
		outUIElement.SetScale(scale);
	}
}

bool XMLLoader::InitializeEntityUsingXMLFile(Entity& entity,
	const String& pathToXML)
{
	// Load the file
	XMLDoc doc;
	bool isSuccessful = LoadXMLFile(pathToXML, doc);

	if (isSuccessful)
	{
		XMLElement* root = doc.RootElement();

		// Load root entity attributes
		const String actorType = root->Attribute(XMLConsts::TYPE_ATTR);
		const float moveSpeed = root->FloatAttribute(XMLConsts::MOVE_SPEED_ATTR);
		const float baseHealth = root->FloatAttribute(XMLConsts::ENTITY_BASE_HEALTH_ATTR);
		const float baseStamina = root->FloatAttribute(XMLConsts::ENTITY_BASE_STAMINA_ATTR);
		const float baseDamage = root->FloatAttribute(XMLConsts::ENTITY_BASE_DAMAGE_ATTR);
		const float staminaRegenerateSpeed = root->FloatAttribute(XMLConsts::ENTITY_STAMINA_REGENARATE_SPEED_ATTR);
		const float staminaRegenerateDelay = root->FloatAttribute(XMLConsts::ENTITY_STAMINA_REGENARATE_DELAY_ATTR);
		const float	knockBackStrenght = root->FloatAttribute(XMLConsts::ENTITY_KNOCK_BACK_STRENGHT);

		std::ostringstream strStream;
		strStream << actorType << entity.GetId();
		entity.setName(strStream.str());
		entity.SetBaseMoveSpeed(moveSpeed);
		entity.SetBaseDamage(baseDamage);
		entity.SetBaseHealth(baseHealth);
		entity.SetBaseStamina(baseStamina);
		entity.SetStaminaRegenerateSpeed(staminaRegenerateSpeed);
		entity.SetStaminaRegenerateDelay(staminaRegenerateDelay);
		entity.SetKnockBackStrenght(knockBackStrenght);

		// Load entity components
		for (XMLElement* element = root->FirstChildElement(); element != nullptr;
			element = element->NextSiblingElement())
		{
			// Check all actor components
			const String componentType = element->Attribute(XMLConsts::TYPE_ATTR);
			isSuccessful &= InitializeComponent(entity, *element, componentType);
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
			if (nodeName == XMLConsts::INPUT_AXIS)
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
			else if (nodeName == XMLConsts::INPUT_ACTION_BUTTON)
			{
				LoadActionTriggers(gameInput, pElem,
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadActionButton(gameInput, GameInputType::KEYBOARD, pElem,
						actionName, XMLConsts::INPUT_KEY_CODE_ATTR);
				},
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadActionButton(gameInput, GameInputType::MOUSE, pElem,
						actionName, XMLConsts::INPUT_BUTTON_CODE_ATTR);
				},
					[](GameInput& gameInput, const XMLElement* pElem, const String& actionName)
				{
					LoadActionButton(gameInput, GameInputType::GAME_CONTROLLER, pElem,
						actionName, XMLConsts::INPUT_BUTTON_CODE_ATTR);
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
		const XMLElement* rootElement = doc.RootElement();
		if (rootElement != nullptr)
		{
			const XMLElement* bgElement = rootElement->FirstChildElement(XMLConsts::NODE_BACKGROUND);
			const XMLElement* fgElement = rootElement->FirstChildElement(XMLConsts::NODE_FOREGROUND);
			if (bgElement != nullptr && fgElement != nullptr)
			{
				UIElementConfig bgConfig;
				LoadUIElement(*bgElement, bgConfig);

				UIElementConfig fgConfig;
				LoadUIElement(*fgElement, fgConfig);

				float animationSpeed = rootElement->FloatAttribute(XMLConsts::UI_ANIMATION_SPEED_ATTR);
				float minValue = rootElement->FloatAttribute(XMLConsts::UI_MIN_VALUE);
				float maxValue = rootElement->FloatAttribute(XMLConsts::UI_MAX_VALUE);

				healthBar.Init(bgConfig, fgConfig);
				healthBar.SetAnimationSpeed(animationSpeed);
			}
		}
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
		if (root != nullptr)
		{
			const XMLElement* sprite = root->FirstChildElement(XMLConsts::NODE_SPRITE);
			const String& pathToSprite = sprite->Attribute(XMLConsts::PATH_ATTR);
			LoadNodeComponents(world, *root);
			world.Init(pathToSprite);
		}		
	}

	return isSuccessful;
}

void XMLLoader::CreatePhysicsBodyFromAttributes(const XMLElement& xmlElement, PhysicsBodyConfig& outConfig)
{
	const XMLElement* physicsBodyElem = xmlElement.FirstChildElement(XMLConsts::NODE_PHYSICS_BODY);
	const String bodyType = physicsBodyElem->Attribute(XMLConsts::SHAPE_ATTR);

	const float width = physicsBodyElem->FloatAttribute(XMLConsts::WIDTH_ATTR);
	const float height = physicsBodyElem->FloatAttribute(XMLConsts::HEIGHT_ATTR);
	const cocos2d::Size& bodySize = cocos2d::Size(width, height);

	const bool isGravityEnabled = physicsBodyElem->BoolAttribute(XMLConsts::PHYSICS_GRAVITY_ATTR);
	const bool isBodyDynamic = physicsBodyElem->BoolAttribute(XMLConsts::PHYSICS_DYNAMIC_BODY_ATTR);
	const bool isRotationEnabled = physicsBodyElem->BoolAttribute(XMLConsts::PHYSICS_ROTATION_ENABLED_ATTR);
	const int collisionBitMask = physicsBodyElem->IntAttribute(XMLConsts::PHYSICS_BIT_MASK_ATTR);
	const int collisonCategory = physicsBodyElem->IntAttribute(XMLConsts::PHYSICS_COLLISION_CATEGORY);

	
	if (bodyType == XMLConsts::PHYSICS_BODY_BOX_ATTR)
	{
		outConfig.SetBodyType(BodyType::BOX);
	}
	else if(bodyType == XMLConsts::PHYSICS_BODY_EDGE_BOX_ATTR)
	{
		outConfig.SetBodyType(BodyType::EDGE_BOX);
	}
	else
	{
		CC_ASSERT(false && "XMLLoader::CreatePhysicsBodyFromAttributes - Error unknown body type !");
	}
	
	cocos2d::PhysicsMaterial material;
	LoadPhysicsMaterialFromAttributes(xmlElement, material);
	outConfig.SetSize(bodySize);
	outConfig.SetPhysicsMaterial(material);	
	outConfig.SetCollisionBitMask(collisionBitMask);
	outConfig.SetCollisionCategory(collisonCategory);
	outConfig.SetIsDynamic(isBodyDynamic);
	outConfig.SetIsGravityEnabled(isGravityEnabled);
	outConfig.SetRotationEnabled(isRotationEnabled);
}

void XMLLoader::LoadNodeComponents(cc::Node& node, const XMLElement& root)
{
	for (const XMLElement* element = root.FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		const XMLAttribute* attribute =  element->FindAttribute(XMLConsts::TYPE_ATTR);
		if (attribute != nullptr)
		{		
			const String& componentType = attribute->Value();
			if (componentType == GameConsts::RIGID_BODY_COMPONENT)
			{
				PhysicsBodyConfig config;
				CreatePhysicsBodyFromAttributes(*element, config);
				PhysicsManager::AddPhysicsBody(node, config);
				
				cc::EventDispatcher* dispatcher = node.getEventDispatcher();
				Vector2 anchorPos;
				if (dispatcher != nullptr && GetAnchorPosition(*element, anchorPos))
				{
					dispatcher->dispatchCustomEvent(PhysicsManager::GetEventOnPhysicsBodyAnchorSet(),
						&anchorPos);
				}				
			}
		}
	
	}
}

GameInputType XMLLoader::StrToGameInputType(const String& inputType)
{
	GameInputType type = GameInputType::NONE;
	if (inputType == XMLConsts::INPUT_TYPE_KEYBOARD)
	{
		type = GameInputType::KEYBOARD;
	}
	else if (inputType == XMLConsts::INPUT_TYPE_MOUSE)
	{
		type = GameInputType::MOUSE;
	}
	else if (inputType == XMLConsts::INPUT_TYPE_GAME_CONTROLLER)
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
	if (element != nullptr)
	{
		const char* value = element->Attribute(attributeName.c_str());
		if (value != nullptr)
		{
			outValue = value;
		}
	}
}

void XMLLoader::LoadPhysicsMaterialFromAttributes(const XMLElement& element, cocos2d::PhysicsMaterial& outMaterial)
{
	const XMLElement* materialElement = element.FirstChildElement(XMLConsts::NODE_PHYSICS_MATERIAL);
	if (materialElement != nullptr)
	{
		const float density = materialElement->FloatAttribute(XMLConsts::PHYSICS_DESITY_ATTR);
		const float restitution = materialElement->FloatAttribute(XMLConsts::PHYSICS_RESTITUTION_ATTR);
		const float friction = materialElement->FloatAttribute(XMLConsts::PHYSICS_FRICTION_ATTR);

		outMaterial.density = density;
		outMaterial.restitution = restitution;
		outMaterial.friction = friction;
	}
}

void XMLLoader::GetVector3FromElement(const XMLElement& element, Vector3& outResult)
{
	outResult.x = element.FloatAttribute(XMLConsts::AXIS_X_ATTR);
	outResult.y = element.FloatAttribute(XMLConsts::AXIS_Y_ATTR);
	outResult.z = element.FloatAttribute(XMLConsts::AXIS_Z_ATTR);
}

void XMLLoader::GetVector2FromElement(const XMLElement& element, Vector2& outResult)
{
	outResult.x = element.FloatAttribute(XMLConsts::AXIS_X_ATTR);
	outResult.y = element.FloatAttribute(XMLConsts::AXIS_Y_ATTR);
}