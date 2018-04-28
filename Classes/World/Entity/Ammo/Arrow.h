#pragma once

#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

class Arrow: public Entity
{
public:
	static Arrow* Create(const String& pathToSprite, 
		const Vector2& startPosition,
		const Vector2& moveDirection,
		float maxFlyDistance, float moveSpeed);

	virtual void update(float deltaTime) override;

private:
	Arrow(Vector2 startPosition, float maxFlyDistance);

	bool Init(const String& pathToSprite,		
		const Vector2& moveDirection,
		float moveSpeed);
	void RotateArrowInDirectionOfMovement();

private:
	Vector2 m_startPosition;
	float	m_maxFlyDistance;
};

NS_LIGHTSOULS_END
