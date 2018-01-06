#pragma once

static constexpr float PI = 3.14159265359f;
static constexpr int MAX_SPRITE_NAME_LENGTH = 100;

static constexpr int ACTION_MOVE_TAG = 0;
static constexpr int ACTION_ANIM_TAG = 1;

// XML entity components
static constexpr char* XML_PLAYER_ANIM_COMPONENT = "PlayerAnimComponent";
static constexpr char* XML_TRANSFORM_COMPONENT = "TransformComponent";
static constexpr char* XML_RIGID_BODY_COMPONENT = "RigidBodyComponent";
static constexpr char* XML_PLAYER_CONTROLLER_COMPONENT = "PlayerControllerComponent";
static constexpr char* XML_MIRROR_SPRITE_COMPONENT = "MirrorSpriteComponent";

// XML entity attributes
static constexpr char* XML_ENTITY_MOVE_SPEED = "moveSpeed";
static constexpr char* XML_ENTITY_DODGE_SPEED = "dodgeSpeed";
static constexpr char* XML_ENTITY_DODGE_TIME = "dodgeTime";

// XML animation component specifics
static constexpr char* XML_SPRITE_FRAME = "SpriteFrame";

// XML rigid body specifics
static constexpr char* XML_PHYSICS_MATERIAL = "PhysicsMaterial";
static constexpr char* XML_PHYSICS_BODY = "PhysicsBody";

// XML attributes
static constexpr char* XML_NAME_ATTR = "name";
static constexpr char* XML_TYPE_ATTR = "type";

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
