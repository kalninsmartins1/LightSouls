/*!
 * \Author	Martins Kalnins
 * \Date	17.09.2019
 * \Brief	Common interface for all UI
 *			 element attributes.
*/

#pragma once

#include <memory>

namespace UI
{
	class Element;

	class IConfigAttribute
	{
	public:
		virtual int GetPriority() const = 0;

		virtual void Apply(Element& target) const = 0;

		/*
		 * Returns config created in dynamic memory,
		 *	so it is expected that this memory will
		 *	be managed by invoker.
		*/
		virtual std::unique_ptr<IConfigAttribute> Clone() const = 0;
	};
}
