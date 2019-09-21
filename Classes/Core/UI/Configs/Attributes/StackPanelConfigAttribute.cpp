#include "StackPanelConfigAttribute.h"
#include "Classes/Core/UI/Element.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"

using namespace UI;

String StackPanelConfigAttribute::s_type = "StackPanelAttribute";

const String UI::StackPanelConfigAttribute::GetType()
{
	return s_type;
}

std::unique_ptr<IConfigAttribute> UI::StackPanelConfigAttribute::Clone() const
{
	return std::unique_ptr<IConfigAttribute>(std::make_unique<StackPanelConfigAttribute>(*this));
}

void UI::StackPanelConfigAttribute::Apply(Element& target) const
{
	Vector2 largestSize;
	// Add all the elements on top of each other
	for (auto& elemConfig : GetElementConfigs())
	{
		auto elem = elemConfig.Create();
		if (elem != nullptr)
		{
			target.AddChild(*elem);
			elem->SetPos(Vector2::GetZero());
			Vector2 curSize;
			elem->GetSize(curSize);
			if (curSize.GetLenghtSquared() > largestSize.GetLenghtSquared())
			{
				largestSize = curSize;
			}
		}
	}

	target.SetSize(largestSize);
}
