#include "Projectile.h"
#include "ProjectileConfig.h"
#include "World/Physics/PhysicsManager.h"
#include "Classes/World/Entity/Entity.h"
#include "Utils/Utils.h"

Projectile::Projectile(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange)
	: m_shooterEntity(shooter)
	, m_config(config)	
	, m_shootDirection(shootDirection)
	, m_startPosition(shooter.GetPos())
	, m_attackRange(attackRange)
	, m_damage(shooter.GetDamage())
	, m_extraSpriteOne(nullptr)
	, m_extraSpriteTwo(nullptr)
	, m_extraSpriteFadeTime(0.15f)
{
	
}

const Entity& Projectile::GetShooterEntity() const
{
	return m_shooterEntity;
}

float Projectile::GetDamage() const
{
	return m_damage;
}

Projectile* Projectile::Create(const Entity& shooter, const ProjectileConfig& config, const Vector2& shootDirection, float attackRange)
{
	Projectile* projectile = new (std::nothrow) Projectile(shooter, config, shootDirection, attackRange);
	if (projectile != nullptr && projectile->Init())
	{
		projectile->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(projectile);
	}

	return projectile;
}

bool Projectile::Init()
{
	bool isSuccessfull = initWithFile(m_config.GetPathToSprite().GetCStr());
	PhysicsManager::AddPhysicsBody(*this, m_config.GetPhysicsBodyConfig());
	setPositionX(m_startPosition.GetX());
	setPositionY(m_startPosition.GetY());
	_physicsBody->setRotationOffset(90.0f); // For some reason physics body rotation is always offset
	RotateProjectileInDirectionOfMovement();

	return isSuccessfull;
}

void Projectile::InitProjectilePastFrameAnimation()
{
	using namespace cocos2d;

	m_extraSpriteOne = Sprite::create(m_config.GetPathToSprite().GetCStr());
	m_extraSpriteOne->setPosition(getPosition());
	m_extraSpriteOne->setRotation(getRotation());
	m_extraSpriteTwo = Sprite::create(m_config.GetPathToSprite().GetCStr());
	m_extraSpriteTwo->setOpacity(0.0f);

	cc::Node* parent = getParent();
	parent->addChild(m_extraSpriteOne);
	parent->addChild(m_extraSpriteTwo);

	m_extraSpriteOne->setCameraMask(parent->getCameraMask());
	m_extraSpriteTwo->setCameraMask(parent->getCameraMask());
	
	StartSpriteFadeOut(m_extraSpriteOne);
	auto sequence = Sequence::create(DelayTime::create(0.1f), CallFunc::create(CC_CALLBACK_0(Projectile::OnSpriteFaded, this)), nullptr);
	m_extraSpriteTwo->runAction(sequence);
}

void Projectile::update(float deltaTime)
{
	Vector2 curPosition(getPositionX(), getPositionY());
	float distanceCoveredSqrt = (m_startPosition - curPosition).GetLenghtSquared();
	if (distanceCoveredSqrt > (m_attackRange*m_attackRange))
	{
		Destroy();
	}
	else
	{
		Vector2 finalPos = curPosition + m_shootDirection * m_config.GetMoveSpeed() * deltaTime;
		setPositionX(finalPos.GetX());		
		setPositionY(finalPos.GetY());
		RotateProjectileInDirectionOfMovement();
	}
}

void Projectile::setParent(cc::Node* parent) // CC classes should be pushed out to adapters
{
	cc::Node::setParent(parent);
	if (parent != nullptr)
	{
		InitProjectilePastFrameAnimation();
	}
}

void Projectile::Destroy()
{
	_parent->removeChild(m_extraSpriteOne);
	_parent->removeChild(m_extraSpriteTwo);
	_parent->removeChild(this);
}

void Projectile::RotateProjectileInDirectionOfMovement()
{
	const Vector2 up = Vector2(0, 1.0f);
	const float angleBetweenVectors = -Utils::GetSignedAngleBetweenVectors(up, m_shootDirection);
	setRotation(angleBetweenVectors);
}

void Projectile::OnSpriteFaded()
{
 	const Vector2& curPosition = Vector2(getPositionX(), getPositionY());
	const float& curRotation = getRotation();

	if (m_extraSpriteOne->getOpacity() <= 0)
	{
		m_extraSpriteOne->setPositionX(curPosition.GetX());
		m_extraSpriteOne->setPositionY(curPosition.GetY());
		m_extraSpriteOne->setRotation(curRotation);
		m_extraSpriteOne->setOpacity(GameConsts::FULL_OPAQUE);
		StartSpriteFadeOut(m_extraSpriteOne);
	}
	else
	{
		m_extraSpriteTwo->setPositionX(curPosition.GetX());
		m_extraSpriteTwo->setPositionY(curPosition.GetY());
		m_extraSpriteTwo->setRotation(curRotation);
		m_extraSpriteTwo->setOpacity(GameConsts::FULL_OPAQUE);
		StartSpriteFadeOut(m_extraSpriteTwo);
	}
}

void Projectile::StartSpriteFadeOut(cc::Sprite* sprite)
{
	using namespace cocos2d;
	auto sequence = Sequence::create(FadeOut::create(m_extraSpriteFadeTime),
		CallFunc::create(CC_CALLBACK_0(Projectile::OnSpriteFaded, this)),
		nullptr);
	sprite->runAction(sequence);
}
