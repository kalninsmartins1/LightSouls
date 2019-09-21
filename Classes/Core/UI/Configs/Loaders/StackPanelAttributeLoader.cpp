#include "StackPanelAttributeLoader.h"
#include "ElementConfigLoader.h"
#include "Classes/Core/UI/Configs/Attributes/StackPanelConfigAttribute.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/XML/XMLTypes.h"
#include "Utils/XML/XMLLoader.h"

using namespace UI;

std::unique_ptr<IAttributeLoader> UI::StackPanelAttributeLoader::Clone() const
{
	return std::unique_ptr<IAttributeLoader>(std::make_unique<StackPanelAttributeLoader>(*this));
}

bool UI::StackPanelAttributeLoader::LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig)
{
	const XMLElement* stackPanelElem = xmlElem.FirstChildElement(StackPanelConfigAttribute::GetType().GetCStr());
	bool isSuccessful = false;

	if (stackPanelElem != nullptr)
	{
		std::vector<ElementConfig> configs;
		XMLLoader::LoadElementConfigs(*stackPanelElem, configs);

		StackPanelConfigAttribute attr;
		attr.SetContent(configs);
		outConfig.AddAttribute(attr);
		isSuccessful = true;
	}

	return isSuccessful;
}
