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

// XML animation component specifics
static constexpr char* XML_SPRITE_FRAME = "SpriteFrame";

// XML rigid body specifics
static constexpr char* XML_PHYSICS_MATERIAL = "PhysicsMaterial";
static constexpr char* XML_PHYSICS_BODY = "PhysicsBody";
