#include "ProgressBarComponent.h"
#include "Classes/Core/Events/ValueChangedEventData.h"
#include "Classes/Core/UI/Element.h"
#include "Classes/Core/Math/Vector2.h"

using namespace UI;

void ProgressBarComponent::SetCurrentValue(float value)
{
	if (value <= 1.0f && value >= 0.0f)
	{
		SetTargetValue(value);
		m_isAnimating = true;
	}
	else
	{
		CCASSERT(false, "Progress bar only accepts values in rage of 0 to 1 !");
	}	
}

void ProgressBarComponent::MultiplyAnimationSpeed(float multiplier)
{
	m_animationSpeed *= multiplier;
}

void ProgressBarComponent::SetAnimationSpeed(float animationSpeed)
{
	m_animationSpeed = animationSpeed;
}

ProgressBarComponent::ProgressBarComponent(Element& owner)
	: ElementComponent(owner)
	, m_animationSpeed(0)
	, m_currentValue(0)
	, m_targetValue(0)
	, m_fullProgressValue(0)
	, m_isAnimating(false)
	, m_isAnimationgForward(true)
{
}

ProgressBarComponent* UI::ProgressBarComponent::Create(Element& owner)
{
	ProgressBarComponent* comp = new (std::nothrow) ProgressBarComponent(owner);
	if (comp != nullptr && comp->Init())
	{
		comp->autorelease();
		owner.AddComponent(*comp);
	}
	else
	{
		CC_SAFE_DELETE(comp);
	}

	return comp;
}

void ProgressBarComponent::SetTargetValue(float value)
{
	m_targetValue = m_fullProgressValue * value;
	if (m_targetValue > m_currentValue)
	{
		m_isAnimationgForward = true;
	}
	else
	{
		m_isAnimationgForward = false;
	}
}

void ProgressBarComponent::FinishAnimating()
{
	m_currentValue = m_targetValue;
	m_isAnimating = false;
}

void ProgressBarComponent::RegisterForEvent(const String& eventName)
{
	auto dispatcher = cc::Director::getInstance()->getEventDispatcher();
	if (dispatcher != nullptr)
	{
		dispatcher->addCustomEventListener(eventName.GetCStr(),
			CC_CALLBACK_1(ProgressBarComponent::OnUpdateEvent, this));
	}
}

void ProgressBarComponent::OnUpdateEvent(cc::EventCustom* eventData)
{
	if (eventData != nullptr)
	{
		auto valueChangeData = static_cast<ValueChangedEventData*>(eventData->getUserData());
		if (valueChangeData != nullptr)
		{
			SetCurrentValue(valueChangeData->GetPercentageNormalized());
		}
	}
}

bool UI::ProgressBarComponent::Init()
{
	bool isSuccessful = ElementComponent::Init();
	Vector2 scale;
	GetOwner().GetScale(scale);
	m_fullProgressValue = scale.GetX();
	m_currentValue = m_fullProgressValue;
	return isSuccessful;
}

void ProgressBarComponent::Update(float deltaTime)
{
	if (m_isAnimating)
	{
		if (m_isAnimationgForward)
		{
			m_currentValue += m_animationSpeed * deltaTime;
			if (m_currentValue >= m_targetValue)
			{
				FinishAnimating();
			}
		}
		else
		{
			m_currentValue -= m_animationSpeed * deltaTime;
			if (m_currentValue <= m_targetValue)
			{
				FinishAnimating();
			}
		}

		GetOwner().SetScaleX(m_currentValue);
	}
}
