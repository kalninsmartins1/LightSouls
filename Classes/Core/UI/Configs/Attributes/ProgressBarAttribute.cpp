#include "ProgressBarAttribute.h"
#include "Classes/Core/UI/Element.h"
#include "Classes/Core/UI/Components/ProgressBarComponent.h"

using namespace UI;

String ProgressBarConfigAttribute::s_type = "ProgressBarAttribute";

const String& UI::ProgressBarConfigAttribute::GetType()
{
	return s_type;
}

int UI::ProgressBarConfigAttribute::GetPriority() const
{
	return 0;
}

void UI::ProgressBarConfigAttribute::SetAnimSpeed(float speed)
{
	m_animSpeed = speed;
}

void UI::ProgressBarConfigAttribute::SetEventName(const String& eventName)
{
	m_eventName = eventName;
}

void UI::ProgressBarConfigAttribute::Apply(Element& target) const
{
	auto* progressBarComp = ProgressBarComponent::Create(target);
	if (progressBarComp != nullptr)
	{
		progressBarComp->SetAnimationSpeed(m_animSpeed);
		progressBarComp->RegisterForEvent(m_eventName);
	}
}

std::unique_ptr<IConfigAttribute> UI::ProgressBarConfigAttribute::Clone() const
{
	return std::unique_ptr<IConfigAttribute>(std::make_unique<ProgressBarConfigAttribute>(*this));
}
