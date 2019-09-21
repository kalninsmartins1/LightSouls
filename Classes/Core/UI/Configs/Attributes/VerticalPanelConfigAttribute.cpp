#include "VerticalPanelConfigAttribute.h"
#include "Classes/Core/UI/Element.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"
#include "Classes/Core/Math/Vector2.h"

using namespace UI;

String VerticalPanelConfigAttribute::s_type = "VerticalPanelAttribute";

const String& UI::VerticalPanelConfigAttribute::GetType()
{
	return s_type;
}

float VerticalPanelConfigAttribute::GetSpacing() const
{
	return m_spacing;
}

float VerticalPanelConfigAttribute::GetLeftPadding() const
{
	return m_leftPadding;
}

float VerticalPanelConfigAttribute::GetTopPadding() const
{
	return m_topPadding;
}

std::unique_ptr<IConfigAttribute> UI::VerticalPanelConfigAttribute::Clone() const
{
	return std::unique_ptr<IConfigAttribute>(std::make_unique<VerticalPanelConfigAttribute>(*this));
}

void UI::VerticalPanelConfigAttribute::SetSpacing(float spacing)
{
	m_spacing = spacing;
}

void UI::VerticalPanelConfigAttribute::SetLeftPadding(float padding)
{
	m_leftPadding = padding;
}

void UI::VerticalPanelConfigAttribute::SetTopPadding(float padding)
{
	m_topPadding = padding;
}

void VerticalPanelConfigAttribute::Apply(Element& target) const
{
	InstantiateChildElements(target);
	ValidateChildPositions(target);
}

float UI::VerticalPanelConfigAttribute::GetTotalChildHeight(const std::vector<Element*>& childElements) const
{
	float totalChildHeight = 0.0f;
	for (auto& child : childElements)
	{
		if (child != nullptr)
		{
			Vector2 childSize;
			child->GetSize(childSize);
			totalChildHeight += childSize.GetY();
		}
	}

	return totalChildHeight;
}

float UI::VerticalPanelConfigAttribute::GetLargestChildWidth(const std::vector<Element*>& childElements) const
{
	float largestWidth = 0.0f;
	for (auto& child : childElements)
	{
		if (child != nullptr)
		{
			Vector2 childSize;
			child->GetSize(childSize);
			float curWidth = childSize.GetX();
			if (curWidth > largestWidth)
			{
				largestWidth = curWidth;
			}
		}
	}

	return largestWidth;
}

void UI::VerticalPanelConfigAttribute::GetContainerSize(const std::vector<Element*>& childElements, Vector2& outSize) const
{
	int childCount = childElements.size();
	float containerWidth = GetLargestChildWidth(childElements) + m_leftPadding;
	float containerHeight = GetTotalChildHeight(childElements) + (m_spacing * childCount);
	outSize.Set(containerWidth, containerHeight);
}

void UI::VerticalPanelConfigAttribute::InstantiateChildElements(Element& target) const
{
	for (auto& config : GetElementConfigs())
	{
		auto element = config.Create();
		if (element != nullptr)
		{
			target.AddChild(*element);
		}
	}
}

void UI::VerticalPanelConfigAttribute::ValidateChildPositions(Element& target) const
{
	const std::vector<Element*>& childElements = target.FindChildElements();
	Vector2 containerSize;
	GetContainerSize(childElements, containerSize);
	target.SetSize(containerSize);

	float curY = containerSize.GetY() - m_topPadding;
	float x = m_leftPadding;
	for (auto& child : childElements)
	{
		if (child != nullptr)
		{
			child->SetPos(Vector2(x, curY));
			Vector2 elementSize;
			child->GetSize(elementSize);
			curY -= (m_spacing + elementSize.GetY());
		}
	}
}
