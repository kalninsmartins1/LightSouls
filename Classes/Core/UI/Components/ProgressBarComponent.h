#pragma once

// brief: This class represents progress bar
//		  that works with values normalized
//		  in range of 0 to 1.

#include "Classes/Core/UI/ElementComponent.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class ProgressBarComponent : public ElementComponent
	{
	public:
		ProgressBarComponent(Element& owner);

		static ProgressBarComponent* Create(Element& owner);

		void SetAnimationSpeed(float animationSpeed);
		void SetCurrentValue(float currentValue);

		virtual bool Init() override;
		virtual void Update(float deltaTime) override;

		void MultiplyAnimationSpeed(float multiplier);
		void RegisterForEvent(const String& eventName);

	private:
		void SetTargetValue(float value);
		void FinishAnimating();
		void OnUpdateEvent(cc::EventCustom* eventData);

	private:
		float			 m_animationSpeed;
		float			 m_currentValue;
		float			 m_targetValue;
		float			 m_fullProgressValue;
		bool			 m_isAnimating;
		bool			 m_isAnimationgForward;
	};
};

