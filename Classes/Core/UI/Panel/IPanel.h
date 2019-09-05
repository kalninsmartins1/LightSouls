/*!
 * \Author	Martins Kalnins
 * \Date	20.07.2019
 * \Brief	Interface for container that can draw a background and
 *				hold multiple child Elements.
*/
#pragma once

#include "Classes/Core/UI/IElement.h"

namespace UI
{
	class ElementConfig;

	class IPanel : public IElement
	{
	public:
		virtual bool Init(const ElementConfig& elementConfig) = 0;
		virtual void AddElement(IElement& element) = 0;
	};
}