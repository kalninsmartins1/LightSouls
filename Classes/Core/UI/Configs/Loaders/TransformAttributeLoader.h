/*!
 * \Author	Martins Kalnins
 * \Date	21.09.2019
 * \Brief	UI element transform config attribute loader.
*/

#pragma once

#include "IAttributeLoader.h"

namespace UI
{
	class TransformAttributeLoader : public IAttributeLoader
	{
	public:
		virtual std::unique_ptr<IAttributeLoader>	Clone() const override;
		virtual bool								LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig) override;
	};
}