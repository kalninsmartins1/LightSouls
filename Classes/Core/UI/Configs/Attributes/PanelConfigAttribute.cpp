#include "PanelConfigAttribute.h"

using namespace UI;

const std::vector<ElementConfig>& PanelConfigAttribute::GetElementConfigs() const
{
	return m_content;
}

void UI::PanelConfigAttribute::SetContent(const std::vector<ElementConfig>& content)
{
	m_content = content;
}

void UI::PanelConfigAttribute::AddContent(const ElementConfig& config)
{
	m_content.emplace_back(config);
}

int UI::PanelConfigAttribute::GetPriority() const
{
	return 0;
}
