#pragma once

#include "LightSoulsTypes.h"

class LSAnimation : public cocos2d::Animation
{
public:
	LSAnimation(const std::vector<float>& frameDelays);

public:
	static LSAnimation* Create(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames, std::vector<float> frameDelays, bool shouldReverse = false);

	virtual float		getDuration() const override;

private:
	bool Init(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames, bool shouldReverse);

private:
	std::vector<float> m_frameDelays;
};