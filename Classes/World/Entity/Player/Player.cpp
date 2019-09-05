#include "Player.h"
#include "Utils/Utils.h"
#include "../Components/AnimComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Classes/Core/Input/GameInput.h"
#include "physics3d/CCPhysics3DWorld.h"
#include "World/Physics/PhysicsManager.h"
#include "Classes/Core/Events/ValueChangedEventData.h"
#include "cocos2d/cocos/base/CCEventDispatcher.h"
#include "Utils/AnimationUtils.h"
#include "Classes/Core/Camera/Components/CameraShake.h"
#include "Scenes/GameScene.h"
#include "World/Projectiles/Projectile.h"
#include "Classes/World/VFX/VFX.h"
#include "Classes/Core/Events/PositionEventData.h"
#include "World/Entity/Components/Attack/LongSwordAttackComponent.h"
#include "World/Cursor.h"

const String Player::s_eventOnPlayerHealthChanged = "EVENT_ON_PLAYER_HEALTH_CHANGED";
const String Player::s_eventOnPlayerStaminaChanged = "EVENT_ON_PLAYER_STAMINA_CHANGED";
const String Player::s_eventOnPlayerGiveDamage = "EVENT_ON_PLAYER_GIVE_DAMAGE";
const String Player::s_eventOnPlayerDodged = "EVENT_ON_PLAYER_DODGED";
const String Player::s_eventOnPlayerDisappeared = "EVENT_ON_PLAYER_DISAPPEARED";

Player* Player::Create(const String& pathToXML)
{
	Player* player = new (std::nothrow) Player();
	if (player != nullptr && player->Init(pathToXML))
	{
		player->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(player);
	}

	return player;
}

Player::Player()
	: Entity()
	, m_attackComponent(nullptr)
	, m_lastValidMoveDirection(Vector2::GetOneX()) // By default start out moving right	
	, m_cursor(nullptr)
	, m_isDodging(false)
	, m_dodgeSpeed(0.0f)
	, m_dodgeTime(0.0f)
	, m_timeBetweenComboInput(0.0f)
	, m_dodgeStaminaConsumption(0.0f)
	, m_isCollidedFromLeft(false)
	, m_isCollidedFromRight(false)
	, m_isCollidedFromTop(false)
	, m_isCollidedFromBottom(false)
{

}

const String& Player::GetEventOnHealthChanged()
{
	return s_eventOnPlayerHealthChanged;
}

const String& Player::GetEventOnStaminaChanged()
{
	return s_eventOnPlayerStaminaChanged;
}

bool Player::Init(const String& pathToXML)
{
	bool isSuccessfullyInitialized = true;
	isSuccessfullyInitialized &= XMLLoader::InitializeEntityUsingXMLFile(*this, pathToXML);
	
	OnEntityInitialized();
	isSuccessfullyInitialized &= InitAttackComponent();

	PhysicsManager* physicsManager = GameScene::GetPhysicsManager();
	if (physicsManager != nullptr)
	{
		const String& name = getName();
		physicsManager->AddContactBeginListener(name,
			CC_CALLBACK_2(Player::OnContactBegin, this));
		physicsManager->AddContactEndListener(name,
			CC_CALLBACK_2(Player::OnContactEnd, this));
		physicsManager->AddContactBeginListener(name + GameConsts::NODE_COMPONENT,
			CC_CALLBACK_2(Player::OnProjectileHit, this));
	}
	else
	{
		isSuccessfullyInitialized = false;
	}

	return isSuccessfullyInitialized;
}

void Player::Update(float deltaTime)
{
	if (GetCurrentHealth() > 0)
	{
		// Call base update
		Entity::Update(deltaTime);

		if (m_cursor != nullptr)
		{
			m_cursor->Update(deltaTime);
			SetLookAtDirection(m_cursor->GetLookAtDirection());
		}

		// We can move only when we are not attacking
		if (IsReadyToAttack() && !m_isDodging)
		{
			ManageInput();
		}

		// If still not attacking
		if (IsReadyToAttack())
		{
			PlayRunOrIdleAnimation();
		}
	}
	else
	{
		SetMoveDirection(Vector2::GetZero());
	}

	AnimComponent* animComponent = GetAnimComponent();
	if (animComponent != nullptr)
	{
		animComponent->UpdateAnimState(*this);
	}
}

void Player::setParent(Node * parent)
{
	Entity::setParent(parent);
	if (parent != nullptr && m_cursor != nullptr)
	{
		m_cursor->OnOwnerParented(*parent);
	}
}

const String& Player::GetEventOnGiveDamage()
{
	return s_eventOnPlayerGiveDamage;
}

const String& Player::GetEventOnPlayerDisappeared()
{
	return s_eventOnPlayerDisappeared;
}

EntityType Player::GetEntityType() const
{
	return EntityType::PLAYER;
}

void Player::SetDodgeStaminaConsumption(float dodgeStaminaConumption)
{
	m_dodgeStaminaConsumption = dodgeStaminaConumption;
}

void Player::SetDodgeSpeed(float dodgeSpeed)
{
	m_dodgeSpeed = dodgeSpeed;
}

void Player::SetDodgeTime(float dodgeTime)
{
	m_dodgeTime = dodgeTime;
}

void Player::OnDodgeFinished()
{
	StopDodging();
}

void Player::ManageInput()
{
	GameInput* input = GameScene::GetGameInput();
	if (input->HasAction("LightAttackInput") && m_attackComponent != nullptr)
	{
		Attack(*m_attackComponent);
	}
	else if (input->HasAction("DodgeInput") && IsRunning() && 
		HasEnoughtStamina(m_dodgeStaminaConsumption))
	{
		PerformDodge();
	}

	// Player movement
	const float horizontalValue = input->GetInputAxis("HorizontalMovement");
	const float vertiacalValue = input->GetInputAxis("VerticalMovement");
	Vector2 moveDirection(horizontalValue, vertiacalValue);

	// Make sure we are not moving faster diagonally
	moveDirection.Normalize();
	FilterMovementDirectionBasedOnCollisionData(moveDirection);
	SetMoveDirection(moveDirection);

	// If we have an input, then update last valid move direction
	if (abs(horizontalValue) > MATH_EPSILON || abs(vertiacalValue) > MATH_EPSILON)
	{
		m_lastValidMoveDirection = moveDirection;
	}
}

void Player::FilterMovementDirectionBasedOnCollisionData(Vector2& moveDirection)
{
	if (m_isCollidedFromLeft && moveDirection.GetX() < 0.0f)
	{
		 moveDirection.SetX(0.0f);
	}
	else if (m_isCollidedFromRight && moveDirection.GetX() > 0.0f)
	{
		moveDirection.SetX(0.0f);
	}
	else if (m_isCollidedFromTop && moveDirection.GetY() > 0.0f)
	{
		moveDirection.SetY(0.0f);
	}
	else if (m_isCollidedFromBottom && moveDirection.GetY() < 0.0f)
	{
		moveDirection.SetY(0.0f);
	}
}

void Player::StartDodging()
{
	m_isDodging = true;
	SetCurrentMoveSpeed(0.0f);
	ApplyInstantSpeed(m_dodgeSpeed);
	
	auto pos = getPosition();
	DispatchEvent(s_eventOnPlayerDodged, 
		&PositionEventData(GetId(), Vector2(pos.x, pos.y)));

	// Consume stamina
	ConsumeStamina(m_dodgeStaminaConsumption);
}

void Player::StopDodging()
{
	m_isDodging = false;
	ResetMoveSpeed();
}

void Player::Attack(GenericAttackComponent& attackComponent)
{	
	if (!m_isDodging && attackComponent.IsReadyToAttack())
	{
		attackComponent.Attack(GetLookAtDirection());
		StartAttacking();		
	}
}

void Player::PerformDodge()
{		
	AnimComponent* animComp = GetAnimComponent();
	if (animComp != nullptr)
	{
		StartDodging();
		animComp->PlayLoopingDirectionalAnim(GameConsts::ANIM_TYPE_DODGE_DIR, false);
		Utils::StartTimerWithCallback(this,
			CC_CALLBACK_0(Player::OnDodgeFinished, this), m_dodgeTime);
	}	
}

void Player::PlayRunOrIdleAnimation() const
{		
	auto animComponent = GetAnimComponent();
	if (animComponent != nullptr)
	{
		if (IsRunning() && !m_isDodging)
		{
			if (!animComponent->IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_RUN_DIR))
			{
				animComponent->PlayLoopingDirectionalAnim(GameConsts::ANIM_TYPE_RUN_DIR);
			}
		}
		else if (!IsRunning())
		{
			if (!animComponent->IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_IDLE_DIR))
			{
				animComponent->PlayLoopingDirectionalAnim(GameConsts::ANIM_TYPE_IDLE_DIR);
			}
		}
	}
}

void Player::DispatchOnHealthReduceEvent()
{
	// Dispatch health changed event
	float currentHealth = GetCurrentHealth();
	float healthPercentage = Utils::SafeDivide(currentHealth, GetMaxHealth());
	auto eventData = ValueChangedEventData(GetId(), currentHealth, healthPercentage);
	DispatchEvent(s_eventOnPlayerHealthChanged, &eventData);	
}

void Player::DispatchOnStaminaChangedEvent() const
{
	float currentStamina = GetCurrentStamina();
	float staminaPercentage = Utils::SafeDivide(currentStamina, GetMaxStamina());
	auto eventData = ValueChangedEventData(GetId(), currentStamina, staminaPercentage);
	DispatchEvent(s_eventOnPlayerStaminaChanged, &eventData);	
}

void Player::DispatchOnGiveDamageEvent() const
{
	DispatchEvent(s_eventOnPlayerGiveDamage);	
}

bool Player::OnContactBegin(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody)
{
	cocos2d::Node* otherNode = otherBody->getNode();
	if (otherNode != nullptr)
	{
		SetCollisionData(otherNode);
	}

	return true;
}

bool Player::OnContactEnd(const Vector2& contactPoint, const cc::PhysicsBody* otherBody)
{
	ResetCollisionData();
	return true;
}

bool Player::OnProjectileHit(const Vector2& contactPoint, const cc::PhysicsBody* otherBody)
{
	auto projectile = static_cast<Projectile*>(otherBody->getNode());
	if (projectile != nullptr)
	{
		TakeDamage(projectile->GetDamage());
		projectile->Destroy();
	}

	return false;
}

void Player::ResetCollisionData()
{
	m_isCollidedFromLeft = false;
	m_isCollidedFromRight = false;
	m_isCollidedFromTop = false;
	m_isCollidedFromBottom = false;
}

bool Player::InitAttackComponent()
{	
	bool isSucessfullyInitialized = false;
	m_attackComponent = static_cast<LongSwordAttackComponent*>(getComponent(GameConsts::ATTACK_COMPONENT));

	if (m_attackComponent != nullptr)
	{
		m_attackComponent->SetAttackFinishCallback(CC_CALLBACK_0(Entity::StopAttacking, this));
		isSucessfullyInitialized = true;
	}

	return isSucessfullyInitialized;
}

bool Player::InitCursor(const String& pathToXML)
{
	GameInput* gameInput = GameScene::GetGameInput();
	if (gameInput != nullptr)
	{
		m_cursor = Cursor::Create(*this, *gameInput, pathToXML);
	}

	return m_cursor != nullptr;
}

void Player::DispatchOnDisappeared() const
{
	cc::EventDispatcher* dispatcher = getEventDispatcher();
	if (dispatcher != nullptr)
	{
		dispatcher->dispatchCustomEvent(s_eventOnPlayerDisappeared.GetCStr());
	}
}

void Player::SetCollisionData(cc::Node* otherNode)
{
	if (abs(m_lastValidMoveDirection.GetX()) > abs(m_lastValidMoveDirection.GetY()))
	{
		if (m_lastValidMoveDirection.GetX() < 0)
		{
			m_isCollidedFromLeft = true;
		}
		else
		{
			m_isCollidedFromRight = true;
		}
	}
	else
	{
		if (m_lastValidMoveDirection.GetY() < 0)
		{
			m_isCollidedFromBottom = true;
		}
		else
		{
			m_isCollidedFromTop = true;
		}
	}	
}


