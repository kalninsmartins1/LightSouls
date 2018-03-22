#include "Arrow.h"
#include "Utils/Utils.h"

Arrow* Arrow::Create(const std::string& pathToSprite, 
	const Vector2& startPosition,
	const Vector2& moveDirection,
	float maxFlyDistance, float moveSpeed)
{
	Arrow* arrow = new (std::nothrow) Arrow(startPosition, maxFlyDistance);
	if (arrow != nullptr && arrow->Init(pathToSprite, moveDirection,
		moveSpeed))
	{
		arrow->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(arrow);
	}

	return arrow;
}

void Arrow::update(float deltaTime)
{
	const Vector2& currentPosition = getPosition();
	setPosition(currentPosition + GetHeading() * 
		GetCurrentMoveSpeed() * deltaTime);

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

bool Arrow::Init(const std::string& pathToSprite, 
	const cocos2d::Vec2& moveDirection,
	float moveSpeed)
{
	
	SetMoveDirection(moveDirection);	
	SetBaseMoveSpeed(moveSpeed);
	setPosition(m_startPosition);
	RotateArrowInDirectionOfMovement();
	scheduleUpdate();

	return Entity::initWithFile(pathToSprite);
}

void Arrow::RotateArrowInDirectionOfMovement()
{
	const Vector2& heading = GetHeading();
	const float angleBetweenVectors = CC_RADIANS_TO_DEGREES(acos(
		Vector2(0, 1).dot(heading) / heading.length()));
	setRotation(angleBetweenVectors);
}
