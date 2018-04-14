#include "CameraFallow.h"
#include "2d/CCNode.h"

using namespace cocos2d;

CameraFollow* CameraFollow::Create(Node* followedNode, const Rect& rect)
{
	return CreateWithOffset(followedNode, 0.0, 0.0, rect);
}

CameraFollow* CameraFollow::CreateWithOffset(Node* followedNode, float xOffset, float yOffset, const Rect& rect)
{
	CameraFollow *follow = new (std::nothrow) CameraFollow();

	bool valid = follow->initWithTargetAndOffset(followedNode, xOffset,
		yOffset, rect);

	if (follow && valid)
	{
		follow->autorelease();
		return follow;
	}

	delete follow;
	return nullptr;
}

void CameraFollow::step(float dt)
{
	if (_boundarySet)
	{
		// whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
		if (_boundaryFullyCovered)
		{
			return;
		}

		Vec2 tempPos = _followedNode->getPosition();

		_target->setPosition(clampf(tempPos.x, _leftBoundary, _rightBoundary),
			clampf(tempPos.y, _bottomBoundary, _topBoundary));
	}
	else
	{
		_target->setPosition(_followedNode->getPosition());
	}
}
