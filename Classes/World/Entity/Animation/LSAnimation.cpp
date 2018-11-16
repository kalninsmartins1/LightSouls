#include "LSAnimation.h"

LSAnimation::LSAnimation(const std::vector<float>& frameDelays)
	: m_frameDelays(frameDelays)
{

}

LSAnimation* LSAnimation::Create(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames, std::vector<float> frameDelays, bool shouldReverse /*= false*/)
{
	LSAnimation* animation = new (std::nothrow) LSAnimation(frameDelays);
	animation->Init(frames, shouldReverse);
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

bool LSAnimation::Init(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames, bool shouldReverse)
{	
	_loops = 1;
	int index = 0;

	if (shouldReverse)
	{
		for (auto& it = frames.rbegin(); it != frames.rend(); ++it)
		{
			auto animFrame = cocos2d::AnimationFrame::create(*it, m_frameDelays[index], cocos2d::ValueMap());
			_frames.pushBack(animFrame);
			_totalDelayUnits++;
			index++;
		}
	}
	else
	{
		for (auto& spriteFrame : frames)
		{
			auto animFrame = cocos2d::AnimationFrame::create(spriteFrame, m_frameDelays[index], cocos2d::ValueMap());
			_frames.pushBack(animFrame);
			_totalDelayUnits++;
			index++;
		}
	}

	return index > 0;
}
