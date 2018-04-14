#pragma once

#include "2d/CCAction.h"

class CameraFollow: public cocos2d::Follow
{
public:
	static CameraFollow* Create(cocos2d::Node *followedNode,
		const cocos2d::Rect& rect = cocos2d::Rect::ZERO);
	static CameraFollow* CameraFollow::CreateWithOffset(cocos2d::Node* followedNode,
		float xOffset, float yOffset, const cocos2d::Rect& rect = cocos2d::Rect::ZERO);
	
	virtual void step(float dt) override;
};
