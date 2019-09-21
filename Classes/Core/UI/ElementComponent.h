/*!
 * \Author	Martins Kalnins
 * \Date	03.08.2019
 * \Brief	Class for representing a UI element component
 *				that can be attached to element.
 *			Implements adapter pattern for cc::Component.
*/
#pragma once

#include "Classes/External/CocosEngine.h"

namespace UI
{
	class Element;

	class ElementComponent : public cc::Component
	{
	public:
		ElementComponent(Element& owner);

		Element& GetOwner() const;

		virtual bool Init();
		virtual void Update(float deltaTime);
		virtual void update(float deltaTime) override;

	private:
		Element& m_owner;
	};
}