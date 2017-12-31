#pragma once

static constexpr float PI = 3.14159265359f;
static constexpr int MAX_SPRITE_NAME_LENGTH = 100;

static constexpr int ACTION_MOVE_TAG = 0;
static constexpr int ACTION_ANIM_TAG = 1;

// XML components
static constexpr char* XML_PLAYER_ANIM_COMPONENT = "PlayerAnimComponent";
static constexpr char* XML_TRANSFORM_COMPONENT = "TransformComponent";
static constexpr char* XML_RIGID_BODY_COMPONENT = "RigidBodyComponent";
static constexpr char* XML_PLAYER_CONTROLLER_COMPONENT = "PlayerControllerComponent";
static constexpr char* XML_MIRROR_SPRITE_COMPONENT = "MirrorSpriteComponent";

// XML animation component specifics
static constexpr char* XML_SPRITE_FRAME = "SpriteFrame";

// XML rigid body specifics
static constexpr char* XML_PHYSICS_MATERIAL = "PhysicsMaterial";
static constexpr char* XML_PHYSICS_BODY = "PhysicsBody";

// XML attributes
static constexpr char* XML_NAME_ATTR = "name";
static constexpr char* XML_TYPE_ATTR = "type";
static constexpr char* XML_KEY_CODE_ATTR = "keyCode";
static constexpr char* XML_KEY_CODE_FROM_ATTR = "keyCodeFrom";
static constexpr char* XML_KEY_CODE_TO_ATTR = "keyCodeTo";
static constexpr char* XML_VALUE_FROM_ATTR = "valueFrom";
static constexpr char* XML_VALUE_TO_ATTR = "valueTo";
static constexpr char* XML_BUTTON_CODE_ATTR = "buttonCode";

// Input types
static constexpr char* KEYBOARD_INPUT = "Keyboard";
static constexpr char* MOUSE_INPUT = "Mouse";
static constexpr char* JOYSTICK_INPUT = "Joystick";
