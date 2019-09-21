#include "ImageConfigAttribute.h"
#include "Classes/Core/UI/Element.h"

using namespace UI;

String ImageConfigAttribute::s_type = "ImageAttribute";

const String& UI::ImageConfigAttribute::GetType()
{
	return s_type;
}

int UI::ImageConfigAttribute::GetPriority() const
{
	return 1;
}

const String& ImageConfigAttribute::GetPathToImage() const
{
	return m_pathToImage;
}

std::unique_ptr<IConfigAttribute> UI::ImageConfigAttribute::Clone() const
{
	return std::unique_ptr<IConfigAttribute>(std::make_unique<ImageConfigAttribute>(*this));
}

void ImageConfigAttribute::SetPathToImage(const String& pathToSprite)
{
	m_pathToImage = pathToSprite;
}

void UI::ImageConfigAttribute::Apply(Element& target) const
{
	target.SetImage(m_pathToImage.GetCStr());
}
