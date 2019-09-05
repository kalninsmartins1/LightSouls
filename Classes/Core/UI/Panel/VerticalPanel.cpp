#include "VerticalPanel.h"
#include "IVerticalPanelImpl.h"
#include "Classes/Core/UI/ElementConfig.h"
#include "Classes/Core/Math/Vector2.h"

using namespace UI;

VerticalPanel::VerticalPanel(IVerticalPanelImpl* impl)
	: m_verticalPanelImpl(impl)
{

}

bool VerticalPanel::GetNormalizedPos(Vector2& outVec) const
{
	if (m_verticalPanelImpl != nullptr)
	{
		return m_verticalPanelImpl->GetNormalizedPos(outVec);
	}
	
	return false;
}

bool VerticalPanel::GetPathToImage(String& outPathToImage) const
{
	if (m_verticalPanelImpl != nullptr)
	{
		return m_verticalPanelImpl->GetPathToImage(outPathToImage);
	}

	return false;
}

bool VerticalPanel::Init(const ElementConfig& elementConfig)
{
	if (m_verticalPanelImpl != nullptr)
	{
		return m_verticalPanelImpl->Init(elementConfig);
	}

	return false;
}

void VerticalPanel::AddElement(IElement& element)
{
	if (m_verticalPanelImpl != nullptr)
	{
		m_verticalPanelImpl->AddElement(element);
	}
}
