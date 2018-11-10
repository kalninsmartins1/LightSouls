#include "LSAnimation.h"

LSAnimation::LSAnimation(const std::vector<float>& frameDelays)
	: m_frameDelays(frameDelays)
{

}

LSAnimation* LSAnimation::Create(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames, std::vector<float> frameDelays, unsigned int loops /*= 1*/)
{
	LSAnimation* animation = new (std::nothrow) LSAnimation(frameDelays);
	animation->Init(frames);
	animation->autorelease();

	return animation;
}

float LSAnimation::getDuration() const
{
	float duration = 0.0f;
	for (const float& delay : m_frameDelays)
	{
		duration += delay;
	}

	return duration;
}

bool LSAnimation::Init(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames)
{
	_loops = 1;

	int index = 0;
	for (auto& spriteFrame : frames)
	{
		auto animFrame = cocos2d::AnimationFrame::create(spriteFrame, m_frameDelays[index], cocos2d::ValueMap());
		_frames.pushBack(animFrame);
		_totalDelayUnits++;
		index++;
	}

	return index > 0;
}
