/*!
 * \Author	Martins Kalnins
 * \Date	21.09.2019
 * \Brief	UI element vertical panel config attribute loader.
*/
#pragma once

#include "IAttributeLoader.h"

namespace UI
{
	class VerticalPanelAttributeLoader : public IAttributeLoader
	{
	public:
		virtual std::unique_ptr<IAttributeLoader> Clone() const override;
		virtual bool LoadAttribute(const XMLElement& xmlElem, ElementConfig& outConfig) override;
	};
}