/*!
 * \Author	Martins Kalnins
 * \Date	18.09.2019
 * \Brief	Element configuration attribute
 *				that allows containers to stack
 *				elements.
*/
#pragma once

#include "PanelConfigAttribute.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class StackPanelConfigAttribute : public PanelConfigAttribute
	{
	public:
		static const String							GetType();
		virtual std::unique_ptr<IConfigAttribute>	Clone() const override;

		virtual void Apply(Element& target) const override;
	private:
		static String s_type;
	};
}