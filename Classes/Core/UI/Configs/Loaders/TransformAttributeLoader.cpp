#include "TransformAttributeLoader.h"
#include "Classes/Core/UI/Configs/Attributes/TransformConfigAttribute.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"
#include "Utils/XML/XMLTypes.h"
#include "Utils/XML/XMLConsts.h"
#include "Utils/XML/XMLLoader.h"

using namespace UI;

std::unique_ptr<IAttributeLoader> UI::TransformAttributeLoader::Clone() const
{
	return std::unique_ptr<IAttributeLoader>(std::make_unique<TransformAttributeLoader>(*this));
}

bool UI::TransformAttributeLoader::LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig)
{
	const XMLElement* transformElem = xmlElem.FirstChildElement(TransformConfigAttribute::GetType().GetCStr());
	bool isSuccessful = false;

	if (transformElem != nullptr)
	{
		const XMLElement* anchorPositionElement = transformElem->FirstChildElement(XMLConsts::NODE_ANCHOR_POSITION);
		const XMLElement* normalizedPositionElement = transformElem->FirstChildElement(XMLConsts::NODE_NORMALIZED_POSITION);
		const XMLElement* scaleElement = transformElem->FirstChildElement(XMLConsts::NODE_SCALE);

		if (anchorPositionElement != nullptr &&
			normalizedPositionElement != nullptr &&
			scaleElement != nullptr)
		{
			TransformConfigAttribute attr;
			Vector2 anchorPos;
			XMLLoader::GetVector2FromElement(*anchorPositionElement, anchorPos);
			attr.SetAnchorPos(anchorPos);

			Vector2 normalizedPos;
			XMLLoader::GetVector2FromElement(*normalizedPositionElement, normalizedPos);
			attr.SetNormalizedPos(normalizedPos);

			Vector2 scale;
			XMLLoader::GetVector2FromElement(*scaleElement, scale);
			attr.SetScale(scale);
			outConfig.AddAttribute(attr);
			isSuccessful = true;
		}
	}

	return isSuccessful;
}
