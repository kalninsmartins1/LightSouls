#pragma once

static constexpr float PI = 3.14159265359f;
static constexpr int FULL_CIRCLE_DEGREES = 360;
static constexpr int HALF_CIRCLE_DEGREES = 180;
static constexpr int MAX_SPRITE_NAME_LENGTH = 100;

static constexpr int ACTION_MOVE_TAG = 0;
static constexpr int ACTION_ANIM_TAG = 1;

// Entity components
static constexpr char* PLAYER_ANIM_COMPONENT = "PlayerAnimComponent";
static constexpr char* TRANSFORM_COMPONENT = "TransformComponent";
static constexpr char* RIGID_BODY_COMPONENT = "RigidBodyComponent";
static constexpr char* PLAYER_CONTROLLER_COMPONENT = "PlayerControllerComponent";
static constexpr char* MIRROR_SPRITE_COMPONENT = "MirrorSpriteComponent";
static constexpr char* AI_CONTROLLER_COMPONENT = "AIControllerComponent";
static constexpr char* AI_ANIM_COMPONENT = "AIAnimComponent";
static constexpr char* RANGED_ATTACK_COMPONENT = "RangedAttackComponent";
static constexpr char* LONG_SWORD_ATTACK_COMPONENT = "LongSwordAttackComponent";

