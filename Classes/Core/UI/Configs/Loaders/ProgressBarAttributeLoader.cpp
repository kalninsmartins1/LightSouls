#include "ProgressBarAttributeLoader.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"
#include "Classes/Core/UI/Configs/Attributes/ProgressBarAttribute.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/XML/XMLTypes.h"

using namespace UI;

std::unique_ptr<IAttributeLoader> UI::ProgressBarAttributeLoader::Clone() const
{
	return std::unique_ptr<IAttributeLoader>(std::make_unique<ProgressBarAttributeLoader>(*this));
}

bool UI::ProgressBarAttributeLoader::LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig)
{
	const XMLElement* progressBarElem = xmlElem.FirstChildElement(ProgressBarConfigAttribute::GetType().GetCStr());
	bool isSuccessful = false;

	if (progressBarElem != nullptr)
	{
		ProgressBarConfigAttribute attr;
		attr.SetAnimSpeed(progressBarElem->FloatAttribute(XMLConsts::UI_ANIMATION_SPEED_ATTR));
		attr.SetEventName(progressBarElem->Attribute(XMLConsts::UI_EVENT_ATTR));
		outConfig.AddAttribute(attr);
		isSuccessful = true;
	}

	return isSuccessful;
}
