// brief: This class represents progress bar
//		  that works with values normalized
//		  in range of 0 to 1.

#pragma once

#include "cocos2d.h"

class UIElementConfig;

class ProgressBar : public cocos2d::Sprite
{
public:
	void SetAnimationSpeed(float animationSpeed);
	void SetCurrentValue(float currentValue);

	static ProgressBar* Create(const std::string& pathToXML);
	bool				Init(const UIElementConfig& backgroundConfig,
								const UIElementConfig& foregroundConfig);
	void				Update(float deltaTime);

private:
	ProgressBar();

	void SetTargetValue(float value);

	bool Init(const std::string& pathToXML);
	void FinishAnimating();

private:
	cocos2d::Sprite* m_progressLine;
	float			 m_animationSpeed;
	float			 m_currentValue;
	float			 m_targetValue;
	float			 m_fullProgressValue;
	bool			 m_isAnimating;
	bool			 m_isAnimationgForward;
};