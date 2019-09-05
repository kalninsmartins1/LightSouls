/*!
 * \Author	Martins Kalnins
 * \Date	03.08.2019
 * \Brief	Implementation of ui element factory
 *			 interface that provides ability
 *			 to create ui lements with 
 *			 different kinds of implementations.
*/
#pragma once

#include "IElementFactory.h"

namespace UI
{
	class IVerticalPanelImpl;

	class ElementFactory : public IElementFactory
	{
	public:
		ElementFactory(const IVerticalPanelImpl* impl);
		~ElementFactory();

		virtual VerticalPanel CreateVerticalPanel() override;

	private:
		const IVerticalPanelImpl* m_verticalPanelImpl;
	};
};
