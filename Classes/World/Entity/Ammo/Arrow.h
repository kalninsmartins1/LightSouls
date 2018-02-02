#pragma once
#include "World/Entity/Entity.h"

class Arrow: public Entity
{
public:
	static Arrow* create(const std::string& pathToSprite, 
		const Vector2& startPosition,
		const Vector2& moveDirection,
		float maxFlyDistance, float moveSpeed);

	virtual void update(float deltaTime) override;

private:
	Arrow(Vector2 startPosition, float maxFlyDistance);
	bool init(const std::string& pathToSprite,		
		const Vector2& moveDirection,
		float moveSpeed);
	void rotateArrowInDirectionOfMovement();

	Vector2 m_startPosition;
	float m_maxFlyDistance;
};
