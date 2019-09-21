#include "ImageAttributeLoader.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"
#include "Classes/Utils/XML/XMLTypes.h"
#include "Classes/Utils/XML/XMLConsts.h"
#include "Classes/Core/UI/Configs/Attributes/ImageConfigAttribute.h"

using namespace UI;

std::unique_ptr<IAttributeLoader> UI::ImageAttributeLoader::Clone() const
{
	return std::unique_ptr<IAttributeLoader>(std::make_unique<ImageAttributeLoader>(*this));
}

bool ImageAttributeLoader::LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig)
{
	const XMLElement* imageElem = xmlElem.FirstChildElement(ImageConfigAttribute::GetType().GetCStr());
	bool isSuccessful = false;

	if (imageElem != nullptr)
	{
		const String& imagePath = imageElem->Attribute(XMLConsts::PATH_ATTR);
		if (!imagePath.IsEmpty())
		{
			ImageConfigAttribute attr;
			attr.SetPathToImage(imagePath);
			outConfig.AddAttribute(attr);
			isSuccessful = true;
		}
	}

	return isSuccessful;
}
