/*!
 * \Author	Martins Kalnins
 * \Date	03.08.2019
 * \Brief	Interface for ui vertical panel implementation.
 *				This is for realising bridge pattern.
 *				Also adds prototype pattern through
 *				 Clone() function so that instances
 *				 can be cloned through abstract interface.
 *				 
*/
#pragma once

class Vector2;
class String;

namespace UI
{
	class IElement;
	class ElementConfig;

	class IVerticalPanelImpl
	{
	public:
		virtual bool GetNormalizedPos(Vector2& outVec) const = 0;
		virtual bool GetPathToImage(String& outPath) const = 0;

		virtual IVerticalPanelImpl* Clone() const = 0;

		virtual bool Init(const ElementConfig& elementConfig) = 0;
		virtual void AddElement(IElement& element) = 0;
	};
};