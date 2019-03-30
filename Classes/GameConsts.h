#pragma once

namespace GameConsts
{
	// Default values
	static constexpr float			PI = 3.14159265359f;
	static constexpr int			FULL_CIRCLE_DEGREES = 360;
	static constexpr int			HALF_CIRCLE_DEGREES = 180;
	static constexpr int			MAX_SPRITE_NAME_LENGTH = 100;
	static constexpr int			DEFAULT_COLLISION_MASK = 1;
	static constexpr int			WORLD_LAYER = -32768;
	static constexpr int			DEBUG_LAYER = 2;
	static constexpr float			LOADING_TIME = 0.1f;
	static constexpr const char*	DEFAULT_FONT = "Arial";
	static constexpr unsigned char	FULL_OPAQUE = 255;

	// Action tag ids
	static constexpr int ACTION_MOVE = 0;
	static constexpr int ACTION_ANIM = 1;
	static constexpr int ACTION_TIMER = 2;
	static constexpr int ACTION_STAMINA_DELAY = 3;
	static constexpr int ACTION_CAMERA_SHAKE = 4;
	static constexpr int ACTION_COLLISION_CHECK = 5;
	static constexpr int ACTION_PAUSE = 6;
	static constexpr int ACTION_AI_AVOID = 7;
	static constexpr int ACTION_AI_SIGNAL = 8;
	static constexpr int ACTION_TAKE_DAMAGE = 9;
	static constexpr int ACTION_AI_TASK_AVOID = 10;	

	// AI states
	static constexpr const char* AI_STATE_ATTACK = "Attack";
	static constexpr const char* AI_STATE_CHASE = "Chase";
	static constexpr const char* AI_STATE_IDLE = "Idle";
	static constexpr const char* AI_STATE_LINE_ATTACK = "LineAttack";
	static constexpr const char* AI_STATE_PATROL = "Patrol";
	static constexpr const char* AI_STATE_PAUSE = "Pause";
	static constexpr const char* AI_STATE_SIGNALING = "Signaling";
	static constexpr const char* AI_STATE_AVOID = "Avoid";
	static constexpr const char* AI_STATE_IS_PLAYER_CLOSE = "IsPlayerClose";

	// Entity components
	static constexpr char* ANIM_COMPONENT = "AnimComponent";
	static constexpr char* TRANSFORM_COMPONENT = "TransformComponent";
	static constexpr char* RIGID_BODY_COMPONENT = "RigidBodyComponent";
	static constexpr char* PLAYER_CONTROLLER_COMPONENT = "PlayerControllerComponent";
	static constexpr char* MIRROR_SPRITE_COMPONENT = "MirrorSpriteComponent";
	static constexpr char* NODE_COMPONENT = "NodeComponent";
	static constexpr char* AI_CONTROLLER_COMPONENT = "AIControllerComponent";
	static constexpr char* ATTACK_COMPONENT = "AttackComponent";
	static constexpr char* RANGED_ATTACK_COMPONENT = "RangedAttackComponent";
	static constexpr char* LONG_SWORD_ATTACK_COMPONENT = "LongSwordAttackComponent";
	static constexpr char* HIT_ATTACK_COMPONENT = "HitAttackComponent";
	static constexpr char* GENERIC_ATTACK_COMPONENT = "GenericAttackComponent";
	static constexpr char* SPRITE_COMPONENT = "SpriteComponent";
	static constexpr char* BACKGROUND_ACTION_COMPONENT = "BackgroundAction";
	static constexpr char* AI_LOOK_AT_TARGET_COMPONENT = "LookAtAITargetComponent";

	// Entity actions
	static constexpr char* AVOID_TARGET_ACTION = "AIAvoidTarget";

	// Camera components
	static constexpr char* CAMERA_SHAKE_COMPONENT = "CameraShake";
	static constexpr char* CAMERA_FOLLOW_COMPONENT = "CameraFollow";

	// Animation types
	static constexpr char* ANIM_TYPE_ACT = "Act";
	static constexpr char* ANIM_TYPE_RUN = "Run";
	static constexpr char* ANIM_TYPE_IDLE = "Idle";
	static constexpr char* ANIM_TYPE_DODGE = "Dodge";
	static constexpr char* ANIM_TYPE_HURT = "Hurt";
	static constexpr char* ANIM_TYPE_ATTACK = "Attack";
	static constexpr char* ANIM_TYPE_SIGNAL = "Signal";
	
	static constexpr char* ANIM_TYPE_RUN_DIR = "RunSide";	
	static constexpr char* ANIM_TYPE_IDLE_DIR = "IdleSide";	
	static constexpr char* ANIM_TYPE_DODGE_DIR = "DodgeSide";	
	static constexpr char* ANIM_TYPE_HURT_DIR = "HurtSide";	

	static constexpr char* ANIM_TYPE_ATTACK_COMBO_ONE_SIDE = "AttackSide1";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_TWO_SIDE = "AttackSide2";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_THREE_SIDE = "AttackSide3";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FOUR_SIDE = "AttackSide4";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FIVE_SIDE = "AttackSide5";

	static constexpr char* ANIM_TYPE_ATTACK_COMBO_ONE_UP = "AttackUp1";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_TWO_UP = "AttackUp2";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_THREE_UP = "AttackUp3";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FOUR_UP = "AttackUp4";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FIVE_UP = "AttackUp5";

	static constexpr char* ANIM_TYPE_ATTACK_COMBO_ONE_DOWN = "AttackDown1";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_TWO_DOWN = "AttackDown2";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_THREE_DOWN = "AttackDown3";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FOUR_DOWN = "AttackDown4";
	static constexpr char* ANIM_TYPE_ATTACK_COMBO_FIVE_DOWN = "AttackDown5";
};