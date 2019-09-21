/*!
 * \Author	Martins Kalnins
 * \Date	20.09.2019
 * \Brief	Common interface for UI element config attribute loaders.
*/
#pragma once

#include <memory>

namespace tinyxml2
{
	class XMLElement;
}

using XMLElement = tinyxml2::XMLElement; 

namespace UI
{
	class ElementConfig;

	class IAttributeLoader
	{
	public:
		virtual std::unique_ptr<IAttributeLoader> Clone() const = 0;

		virtual bool LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig) = 0;
	};
}
