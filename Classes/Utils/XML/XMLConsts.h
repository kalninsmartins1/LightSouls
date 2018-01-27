#pragma once


// XML nodes
static constexpr char* XML_NODE_PHYSICS_MATERIAL = "PhysicsMaterial";
static constexpr char* XML_NODE_PHYSICS_BODY = "PhysicsBody";
static constexpr char* XML_NODE_AGENT_CONFIG_LIST = "AgentConfigList";

// XML entity components
static constexpr char* XML_PLAYER_ANIM_COMPONENT = "PlayerAnimComponent";
static constexpr char* XML_TRANSFORM_COMPONENT = "TransformComponent";
static constexpr char* XML_RIGID_BODY_COMPONENT = "RigidBodyComponent";
static constexpr char* XML_PLAYER_CONTROLLER_COMPONENT = "PlayerControllerComponent";
static constexpr char* XML_MIRROR_SPRITE_COMPONENT = "MirrorSpriteComponent";
static constexpr char* XML_AI_CONTROLLER_COMPONENT = "AIControllerComponent";
static constexpr char* XML_AI_ANIM_COMPONENT = "AgentAnimationComponent";

// XML entity attributes
static constexpr char* XML_ENTITY_MOVE_SPEED_ATTR = "moveSpeed";
static constexpr char* XML_ENTITY_DODGE_SPEED_ATTR = "dodgeSpeed";
static constexpr char* XML_ENTITY_DODGE_TIME_ATTR = "dodgeTime";

// XML player attributes
static constexpr char* XML_PLAYER_TIME_BETWEEN_COMBO_HIT_ATTR = "timeBetweenComboInput";

// XML ai attributes
static constexpr char* XML_AI_ATTACK_RADIUS_ATTR = "attackRadius";
static constexpr char* XML_AI_WORKING_RADIUS_ATTR = "workingRadius";
static constexpr char* XML_AI_PATROL_PAUSE_ATTR = "patrolPauseInSeconds";

// XML animation component specifics
static constexpr char* XML_ANIM_SPRITE_FRAME = "SpriteFrame";

// XML animation attributes
static constexpr char* XML_ANIM_PLIST_PATH_ATTR = "plistPath";
static constexpr char* XML_ANIM_FRAME_NAME_PATTERN_ATTR = "frameNamePattern";
static constexpr char* XML_ANIM_FRAME_COUNT_ATTR = "frameCount";
static constexpr char* XML_ANIM_TIME_BETWEEN_FRAMES_ATTR = "timeBetweenFrames";

// XML animation type values
static constexpr char* XML_ANIM_TYPE_RUN = "Run";
static constexpr char* XML_ANIM_TYPE_IDLE = "Idle";
static constexpr char* XML_ANIM_TYPE_DODGE = "Dodge";
static constexpr char* XML_ANIM_TYPE_HURT = "Hurt";
static constexpr char* XML_ANIM_TYPE_LIGHT_ATTACK_ONE = "LightAttackOne";
static constexpr char* XML_ANIM_TYPE_LIGHT_ATTACK_TWO = "LightAttackTwo";
static constexpr char* XML_ANIM_TYPE_LIGHT_ATTACK_THREE = "LightAttackThree";
static constexpr char* XML_ANIM_TYPE_LIGHT_ATTACK_FOUR = "LightAttackFour";
static constexpr char* XML_ANIM_TYPE_LIGHT_ATTACK_FIVE = "LightAttackFive";

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
