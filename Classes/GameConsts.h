#pragma once

namespace LightSouls
{
	static constexpr float PI = 3.14159265359f;
	static constexpr int FULL_CIRCLE_DEGREES = 360;
	static constexpr int HALF_CIRCLE_DEGREES = 180;
	static constexpr int MAX_SPRITE_NAME_LENGTH = 100;
	static constexpr int DEFAULT_COLLISION_MASK = 1;

	static constexpr int ACTION_MOVE_TAG = 0;
	static constexpr int ACTION_ANIM_TAG = 1;
	static constexpr int ACTION_TIMER_TAG = 2;

	// Entity components
	static constexpr char* ANIM_COMPONENT = "AnimComponent";
	static constexpr char* TRANSFORM_COMPONENT = "TransformComponent";
	static constexpr char* RIGID_BODY_COMPONENT = "RigidBodyComponent";
	static constexpr char* PLAYER_CONTROLLER_COMPONENT = "PlayerControllerComponent";
	static constexpr char* MIRROR_SPRITE_COMPONENT = "MirrorSpriteComponent";
	static constexpr char* AI_CONTROLLER_COMPONENT = "AIControllerComponent";
	static constexpr char* ATTACK_COMPONENT = "AttackComponent";
	static constexpr char* RANGED_ATTACK_COMPONENT = "RangedAttackComponent";
	static constexpr char* LONG_SWORD_ATTACK_COMPONENT = "LongSwordAttackComponent";

	// Animation types
	static constexpr char* ANIM_TYPE_RUN = "Run";
	static constexpr char* ANIM_TYPE_IDLE = "Idle";
	static constexpr char* ANIM_TYPE_DODGE = "Dodge";
	static constexpr char* ANIM_TYPE_HURT = "Hurt";
	static constexpr char* ANIM_TYPE_ATTACK = "Attack";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_ONE = "AttackCombo1";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_TWO = "AttackCombo2";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_THREE = "AttackCombo3";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FOUR = "AttackCombo4";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FIVE = "AttackCombo5";
};