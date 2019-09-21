#include "VerticalPanelAttributeLoader.h"
#include "Classes/Core/UI/Configs/Attributes/VerticalPanelConfigAttribute.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/XML/XMLTypes.h"
#include "Utils/XML/XMLLoader.h"

using namespace UI;

std::unique_ptr<IAttributeLoader> UI::VerticalPanelAttributeLoader::Clone() const
{
	return std::unique_ptr<IAttributeLoader>(std::make_unique<VerticalPanelAttributeLoader>(*this));
}

bool UI::VerticalPanelAttributeLoader::LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig)
{
	const XMLElement* verticalPanelElem = xmlElem.FirstChildElement(VerticalPanelConfigAttribute::GetType().GetCStr());
	bool isSuccessful = false;

	if (verticalPanelElem != nullptr)
	{
		VerticalPanelConfigAttribute attr;
		attr.SetSpacing(verticalPanelElem->FloatAttribute(XMLConsts::UI_SPACING_ATTR));
		attr.SetLeftPadding(verticalPanelElem->FloatAttribute(XMLConsts::UI_LEFT_PADDING_ATTR));
		attr.SetTopPadding(verticalPanelElem->FloatAttribute(XMLConsts::UI_TOP_PADDING_ATTR));

		std::vector<ElementConfig> configs;
		XMLLoader::LoadElementConfigs(*verticalPanelElem, configs);
		attr.SetContent(configs);

		outConfig.AddAttribute(attr);
		isSuccessful = true;
	}

	return isSuccessful;
}
