#include "LSAnimate.h"
#include "World/Entity/Animation/LSAnimation.h"

LSAnimate* LSAnimate::Create(LSAnimation* animation)
{
	LSAnimate* animate = new (std::nothrow) LSAnimate();
	if (animate && animate->initWithAnimation(animation))
	{
		animate->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(animate);
	}

	return animate;
}

bool LSAnimate::initWithAnimation(cocos2d::Animation *animation)
{
	bool isSuccessfullyInitialised = false;
	CCASSERT(animation != nullptr, "LSAnimate: argument Animation must be non-nullptr");
	if (animation == nullptr)
	{
		CCLOG("LSAnimate::initWithAnimation: argument Animation must be non-nullptr");
	}

	float duration = animation->getDuration();
	if (ActionInterval::initWithDuration(duration * animation->getLoops()))
	{
		_nextFrame = 0;
		setAnimation(animation);
		_origFrame = nullptr;
		_executedLoops = 0;
		_splitTimes->reserve(animation->getFrames().size());

		float accumUnitsOfTime = 0;
		auto& frames = animation->getFrames();

		for (auto& frame : frames)
		{
			float value = accumUnitsOfTime / duration;
			_splitTimes->push_back(value);

			float frameTime = frame->getDelayUnits();
			accumUnitsOfTime += frameTime;
		}
		isSuccessfullyInitialised = true;
	}

	return isSuccessfullyInitialised;
}

