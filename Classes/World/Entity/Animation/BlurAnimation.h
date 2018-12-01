#pragma once

/*
	Class for representing animation blur frame.
*/

#include "LightSoulsTypes.h"
#include "BlurAnimationFrame.h"

class AnimComponent;

class BlurAnimation
{
public:
	BlurAnimation(const AnimComponent& animComp);

public:
	void	AddFrame(float fadeTime, int targetAnimationId, float delay = 0.0f);
	void	Update(float deltaTime);

private:	
	void	LoadNextFrame(BlurAnimationFrame& blurFrame);
	void	RemoveBlurFrame(int index);

private:
	const AnimComponent&			m_animComp;
	std::vector<BlurAnimationFrame> m_blurFrames;	
};