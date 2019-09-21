/*!
 * \Author	Martins Kalnins
 * \Date	20.09.2019
 * \Brief	UI element image config attribute loader.
*/
#pragma once

#include "IAttributeLoader.h"

namespace UI
{
	class ImageAttributeLoader : public IAttributeLoader
	{
	public:
		virtual std::unique_ptr<IAttributeLoader> Clone() const override;
		virtual bool LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig) override;
	};
}