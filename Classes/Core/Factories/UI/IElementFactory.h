/*!
 * \Author	Martins Kalnins
 * \Date	03.08.2019
 * \Brief	Interface for factory that will produce
 *			 different kinds of UI elements.
*/
#pragma once

#include "Classes/Core/UI/Panel/VerticalPanel.h"

namespace UI
{
	class IElementFactory
	{
	public:
		virtual VerticalPanel CreateVerticalPanel() = 0;
	};
};