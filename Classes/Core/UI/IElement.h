/*!
 * \Author	Martins Kalnins
 * \Date	20.07.2019
 * \Brief	Interface for representing an element in UI.
*/
#pragma once

class Vector2;
class String;

namespace UI
{
	class IElement
	{
	public:
		virtual bool GetNormalizedPos(Vector2& outPos) const = 0;
		virtual bool GetPathToImage(String& pathToImage) const = 0;
	};
};