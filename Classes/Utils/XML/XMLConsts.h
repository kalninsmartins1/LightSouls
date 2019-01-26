#pragma once

namespace XMLConsts
{
	// XML nodes
	static constexpr char* NODE_PHYSICS_MATERIAL = "PhysicsMaterial";
	static constexpr char* NODE_PHYSICS_BODY = "PhysicsBody";	
	static constexpr char* NODE_AGENT_CONFIG_LIST = "AgentConfigList";
	static constexpr char* NODE_SPAWN_POINTS = "SpawnPoints";
	static constexpr char* NODE_SPAWN_POINT = "SpawnPoint";
	static constexpr char* NODE_BACKGROUND = "Background";
	static constexpr char* NODE_FOREGROUND = "Foreground";
	static constexpr char* NODE_SPRITE = "Sprite";
	static constexpr char* NODE_SPRITE_FRAME = "SpriteFrame";
	static constexpr char* NODE_NORMALIZED_POSITION = "NormalizedPosition";
	static constexpr char* NODE_ANCHOR_POSITION = "AnchorPosition";
	static constexpr char* NODE_COLLISION_DATA = "CollisionData";
	static constexpr char* NODE_SCALE = "Scale";
	static constexpr char* NODE_POSITION = "Position";
	static constexpr char* NODE_ROTATION = "Rotation";
	static constexpr char* NODE_PROJECTILE = "Projectile";

	// XML entity attributes	
	static constexpr char* ENTITY_SECONDS_BETWEEN_ATTACK_ATTR = "secondsBetweenAttacks";
	static constexpr char* ENTITY_COMBO_EXPIRE_TIME_ATTR = "comboExpireTime";
	static constexpr char* ENTITY_ATTACK_RANGE_ATTR = "attackRange";
	static constexpr char* ENTITY_PADDING_FROM_BODY_ATTR = "paddingFromBody";
	static constexpr char* ENTITY_BASE_HEALTH_ATTR = "baseHealth";
	static constexpr char* ENTITY_BASE_STAMINA_ATTR = "baseStamina";
	static constexpr char* ENTITY_BASE_DAMAGE_ATTR = "baseDamage";
	static constexpr char* ENTITY_STAMINA_REGENARATE_SPEED_ATTR = "staminaRegenerateSpeed";
	static constexpr char* ENTITY_STAMINA_REGENARATE_DELAY_ATTR = "staminaRegenerateDelay";
	static constexpr char* ENTITY_KNOCK_BACK_STRENGHT = "knockBackStrenght";

	// XML UI attributes
	static constexpr char* UI_ANIMATION_SPEED_ATTR = "animationSpeed";
	static constexpr char* UI_MIN_VALUE = "minValue";
	static constexpr char* UI_MAX_VALUE = "maxValue";

	// XML player attributes
	static constexpr char* PLAYER_DODGE_STAMINA_ATTR = "dodgeStaminaConsumption";
	static constexpr char* PLAYER_DODGE_SPEED_ATTR = "dodgeSpeed";
	static constexpr char* PLAYER_DODGE_TIME_ATTR = "dodgeTime";

	// XML attack attributes
	static constexpr char* ATTACK_STAMINA_CONSUMPTION_ATTR = "staminaConsumption";

	// XML AI attributes
	static constexpr char* AI_CHASE_RADIUS_ATTR = "chaseRadius";
	static constexpr char* AI_STOP_DISTANCE = "stopDistance";
	static constexpr char* AI_PATROL_RADIUS_ATTR = "patrolRadius";
	static constexpr char* AI_PATROL_PAUSE_ATTR = "patrolPauseInSeconds";	
	static constexpr char* AI_NEXT_STATE_ON_SUCCESS = "onSuccess";
	static constexpr char* AI_NEXT_STATE_ON_FAILURE = "onFailure";
	static constexpr char* AI_SHOULD_CHECK_DISTANCE_TO_TARGET = "shouldCheckDistanceToTarget";
	static constexpr char* AI_MIN_DISTANCE_TO_TARGET = "minDistanceToTarget";
	static constexpr char* AI_MAX_DISTANCE_TO_TARGET = "maxDistanceToTarget";
	static constexpr char* AI_IS_CHECKING_DISTANCE_TO_TARGET = "isCheckingDistanceToTarget";
	static constexpr char* AI_START_STATE = "startState";
	static constexpr char* AI_SPAWN_DELAY = "spawnDelay";
	static constexpr char* AI_SPAWN_COUNT = "spawnCount";
	static constexpr char* AI_NUM_AGENTS_IN_ROW = "numAgentsInRow";
	static constexpr char* AI_ROW_PADDING = "rowPadding";
	static constexpr char* AI_COLUMN_PADDING = "columnPadding";
	static constexpr char* AI_SKIP_AVOID_STATES = "skipAvoidStates";
	static constexpr char* AI_ARRIVE_DISTANCE = "arriveDistance";
	static constexpr char* AI_DAMAGE_DISTANCE = "damageDistance";
	static constexpr char* AI_FAIL_DISTANCE = "failDistance";

	// XML action attributes
	static constexpr char* START_AVOID_DISTANCE_ATTR = "startAvoidDistance";
	static constexpr char* STOP_AVOID_DISTANCE_ATTR = "stopAvoidDistance";	
	static constexpr char* COLLISION_CHECK_TIME_INTERVAL_ATTR = "collisionCheckTimeInterval";

	// XML animation attributes
	static constexpr char* ANIM_PLIST_PATH_ATTR = "plistPath";
	static constexpr char* ANIM_FRAME_NAME_PATTERN_ATTR = "frameNamePattern";	
	static constexpr char* ANIM_TIME_BETWEEN_FRAMES_ATTR = "timeBetweenFrames";
	static constexpr char* ANIM_SHOULD_BLUR = "shouldBlur";

	// XML generic attributes
	static constexpr char* DISTANCE = "distance";
	static constexpr char* NAME_ATTR = "name";
	static constexpr char* TIME_ATTR = "time";
	static constexpr char* MOVE_SPEED_ATTR = "moveSpeed";
	static constexpr char* TYPE_ATTR = "type";
	static constexpr char* PATH_ATTR = "path";
	static constexpr char* SHAPE_ATTR = "shape";
	static constexpr char* WIDTH_ATTR = "width";
	static constexpr char* HEIGHT_ATTR = "height";
	static constexpr char* AXIS_X_ATTR = "x";
	static constexpr char* AXIS_Y_ATTR = "y";
	static constexpr char* AXIS_Z_ATTR = "z";
	static constexpr char* SENSITIVITY = "sensitivity";

	// XML physics attributes
	static constexpr char* PHYSICS_BIT_MASK_ATTR = "collisionBitMask";
	static constexpr char* PHYSICS_COLLISION_CATEGORY = "collisionCategory";
	static constexpr char* PHYSICS_DYNAMIC_BODY_ATTR = "isDynamic";
	static constexpr char* PHYSICS_ROTATION_ENABLED_ATTR = "isRotationEnabled";
	static constexpr char* PHYSICS_FORCE_SCALE_ATTR = "forceScale";
	static constexpr char* PHYSICS_GRAVITY_ATTR = "isGravityEnabled";
	static constexpr char* PHYSICS_BODY_BOX_ATTR = "box";
	static constexpr char* PHYSICS_BODY_EDGE_BOX_ATTR = "edgeBox";
	static constexpr char* PHYSICS_DESITY_ATTR = "density";
	static constexpr char* PHYSICS_RESTITUTION_ATTR = "restitution";
	static constexpr char* PHYSICS_FRICTION_ATTR = "friction";

	// XML input attributes
	static constexpr char* INPUT_KEY_CODE_ATTR = "keyCode";
	static constexpr char* INPUT_KEY_FROM_ATTR = "keyCodeFrom";
	static constexpr char* INPUT_KEY_TO_ATTR = "keyCodeTo";
	static constexpr char* INPUT_VALUE_FROM_ATTR = "valueFrom";
	static constexpr char* INPUT_VALUE_TO_ATTR = "valueTo";
	static constexpr char* INPUT_BUTTON_CODE_ATTR = "buttonCode";
	static constexpr char* INPUT_AXIS_NAME = "axisCode";

	// XML input types
	static constexpr char* INPUT_AXIS = "Axis";
	static constexpr char* INPUT_ACTION_BUTTON = "ActionButton";
	static constexpr char* INPUT_STATE_BUTTON = "StateButton";

	// XML input device types
	static constexpr char* INPUT_TYPE_KEYBOARD = "Keyboard";
	static constexpr char* INPUT_TYPE_MOUSE = "Mouse";
	static constexpr char* INPUT_TYPE_GAME_CONTROLLER = "GameController";

	// XML camera attributes
	static constexpr char* CAMERA_SHAKE_RADIUS = "shakeRadius";
};

