#include "Arrow.h"
#include "Utils/Utils.h"

Arrow* Arrow::create(const std::string& pathToSprite, 
	const Vector2& startPosition,
	const Vector2& moveDirection,
	float maxFlyDistance, float moveSpeed)
{
	Arrow* pArrow = new (std::nothrow) Arrow(startPosition, maxFlyDistance);
	if (pArrow != nullptr && pArrow->init(pathToSprite, moveDirection,
		moveSpeed))
	{
		pArrow->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pArrow);
	}

	return pArrow;
}

void Arrow::update(float deltaTime)
{
	const Vector2& currentPosition = getPosition();
	setPosition(currentPosition + m_moveDirection * m_moveSpeed * deltaTime);

	if(currentPosition.distance(m_startPosition) >= m_maxFlyDistance)
	{
		getParent()->removeChild(this);		
	}
}

Arrow::Arrow(Vector2 startPosition, float maxFlyDistance) :
	m_startPosition(startPosition),
	m_maxFlyDistance(maxFlyDistance)
{	
}

bool Arrow::init(const std::string& pathToSprite, 
	const cocos2d::Vec2& moveDirection,
	float moveSpeed)
{
	m_moveSpeed = moveSpeed;
	m_moveDirection = moveDirection;
	
	setBaseMoveSpeed(moveSpeed);
	setPosition(m_startPosition);
	rotateArrowInDirectionOfMovement();
	scheduleUpdate();

	return Entity::initWithFile(pathToSprite);
}

void Arrow::rotateArrowInDirectionOfMovement()
{
	const float angleBetweenVectors = CC_RADIANS_TO_DEGREES(acos(
		Vector2(0, 1).dot(m_moveDirection) / m_moveDirection.length()));
	setRotation(angleBetweenVectors);
}
