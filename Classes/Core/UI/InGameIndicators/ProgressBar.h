#pragma once

// brief: This class represents progress bar
//		  that works with values normalized
//		  in range of 0 to 1.

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class ElementConfig;
};


class ProgressBar : public cocos2d::Sprite
{
public:
	void SetAnimationSpeed(float animationSpeed);
	void SetCurrentValue(float currentValue);

	void				MultiplyAnimationSpeed(float multiplier);
	static ProgressBar* Create(const String& pathToXML);
	bool				Init(const UI::ElementConfig& backgroundConfig,
								const UI::ElementConfig& foregroundConfig);
	void				Update(float deltaTime);

private:
	ProgressBar();

	void SetTargetValue(float value);

	bool Init(const String& pathToXML);
	void FinishAnimating();

private:
	cc::Sprite*		 m_progressLine;
	float			 m_animationSpeed;
	float			 m_currentValue;
	float			 m_targetValue;
	float			 m_fullProgressValue;
	bool			 m_isAnimating;
	bool			 m_isAnimationgForward;
};

