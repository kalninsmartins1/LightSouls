#pragma once

namespace LightSouls
{
	// XML nodes
	static constexpr char* XML_NODE_PHYSICS_MATERIAL = "PhysicsMaterial";
	static constexpr char* XML_NODE_PHYSICS_BODY = "PhysicsBody";
	static constexpr char* XML_NODE_AGENT_CONFIG_LIST = "AgentConfigList";
	static constexpr char* XML_NODE_BACKGROUND = "Background";
	static constexpr char* XML_NODE_FOREGROUND = "Foreground";
	static constexpr char* XML_NODE_SPRITE = "Sprite";
	static constexpr char* XML_NODE_SPRITE_FRAME = "SpriteFrame";
	static constexpr char* XML_NODE_NORMALIZED_POSITION = "NormalizedPosition";
	static constexpr char* XML_NODE_ANCHOR_POSITION = "AnchorPosition";
	static constexpr char* XML_NODE_COLLISION_DATA = "CollisionData";
	static constexpr char* XML_NODE_SCALE = "Scale";
	static constexpr char* XML_NODE_POSITION = "Position";
	static constexpr char* XML_NODE_ROTATION = "Rotation";

	// XML entity attributes
	static constexpr char* XML_ENTITY_MOVE_SPEED_ATTR = "moveSpeed";
	static constexpr char* XML_ENTITY_SECONDS_BETWEEN_ATTACK_ATTR = "secondsBetweenAttacks";
	static constexpr char* XML_ENTITY_COMBO_EXPIRE_TIME_ATTR = "comboExpireTime";
	static constexpr char* XML_ENTITY_ATTACK_RANGE_ATTR = "attackRange";
	static constexpr char* XML_ENTITY_PADDING_FROM_BODY_ATTR = "paddingFromBody";
	static constexpr char* XML_ENTITY_BASE_HEALTH_ATTR = "baseHealth";
	static constexpr char* XML_ENTITY_BASE_STAMINA_ATTR = "baseStamina";
	static constexpr char* XML_ENTITY_BASE_DAMAGE_ATTR = "baseDamage";
	static constexpr char* XML_ENTITY_STAMINA_REGENARATE_SPEED_ATTR = "staminaRegenerateSpeed";

	// XML UI attributes
	static constexpr char* XML_UI_ANIMATION_SPEED_ATTR = "animationSpeed";
	static constexpr char* XML_UI_MIN_VALUE = "minValue";
	static constexpr char* XML_UI_MAX_VALUE = "maxValue";

	// XML player attributes
	static constexpr char* XML_PLAYER_DODGE_STAMINA_ATTR = "dodgeStaminaConsumption";
	static constexpr char* XML_PLAYER_DODGE_SPEED_ATTR = "dodgeSpeed";
	static constexpr char* XML_PLAYER_DODGE_TIME_ATTR = "dodgeTime";

	// XML attack attributes
	static constexpr char* XML_ATTACK_STAMINA_CONSUMPTION_ATTR = "staminaConsumption";

	// XML AI attributes
	static constexpr char* XML_AI_CHASE_RADIUS_ATTR = "chaseRadius";
	static constexpr char* XML_AI_CHASE_STOP_DISTANCE = "chaseStopDistance";
	static constexpr char* XML_AI_PATROL_RADIUS_ATTR = "patrolRadius";
	static constexpr char* XML_AI_PATROL_PAUSE_ATTR = "patrolPauseInSeconds";
	static constexpr char* XML_AI_MAX_FLY_DISTANCE = "maxFlyDistance";

	// XML animation attributes
	static constexpr char* XML_ANIM_PLIST_PATH_ATTR = "plistPath";
	static constexpr char* XML_ANIM_FRAME_NAME_PATTERN_ATTR = "frameNamePattern";
	static constexpr char* XML_ANIM_FRAME_COUNT_ATTR = "frameCount";
	static constexpr char* XML_ANIM_TIME_BETWEEN_FRAMES_ATTR = "timeBetweenFrames";

	// XML generic attributes
	static constexpr char* XML_NAME_ATTR = "name";
	static constexpr char* XML_TYPE_ATTR = "type";
	static constexpr char* XML_PATH_ATTR = "path";
	static constexpr char* XML_SHAPE_ATTR = "shape";
	static constexpr char* XML_WIDTH_ATTR = "width";
	static constexpr char* XML_HEIGHT_ATTR = "height";
	static constexpr char* XML_AXIS_X_ATTR = "x";
	static constexpr char* XML_AXIS_Y_ATTR = "y";
	static constexpr char* XML_AXIS_Z_ATTR = "z";

	// XML physics attributes
	static constexpr char* XML_PHYSICS_BIT_MASK_ATTR = "collisionBitMask";
	static constexpr char* XML_PHYSICS_DYNAMIC_BODY_ATTR = "isDynamic";
	static constexpr char* XML_PHYSICS_ROTATION_ENABLED_ATTR = "isRotationEnabled";
	static constexpr char* XML_PHYSICS_FORCE_SCALE_ATTR = "forceScale";
	static constexpr char* XML_PHYSICS_GRAVITY_ATTR = "isGravityEnabled";
	static constexpr char* XML_PHYSICS_BODY_BOX_ATTR = "box";
	static constexpr char* XML_PHYSICS_DESITY_ATTR = "density";
	static constexpr char* XML_PHYSICS_RESTITUTION_ATTR = "restitution";
	static constexpr char* XML_PHYSICS_FRICTION_ATTR = "friction";

	// XML input attributes
	static constexpr char* XML_INPUT_KEY_CODE_ATTR = "keyCode";
	static constexpr char* XML_INPUT_KEY_FROM_ATTR = "keyCodeFrom";
	static constexpr char* XML_INPUT_KEY_TO_ATTR = "keyCodeTo";
	static constexpr char* XML_INPUT_VALUE_FROM_ATTR = "valueFrom";
	static constexpr char* XML_INPUT_VALUE_TO_ATTR = "valueTo";
	static constexpr char* XML_INPUT_BUTTON_CODE_ATTR = "buttonCode";
	static constexpr char* XML_INPUT_AXIS_NAME = "axisCode";

	// XML input types
	static constexpr char* XML_INPUT_AXIS = "Axis";
	static constexpr char* XML_INPUT_ACTION_BUTTON = "ActionButton";
	static constexpr char* XML_INPUT_STATE_BUTTON = "StateButton";

	// XML input device types
	static constexpr char* XML_INPUT_KEYBOARD = "Keyboard";
	static constexpr char* XML_INPUT_MOUSE = "Mouse";
	static constexpr char* XML_INPUT_GAME_CONTROLLER = "GameController";
};

