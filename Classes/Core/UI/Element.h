/*!
 * \Author	Martins Kalnins
 * \Date	03.08.2019
 * \Brief	Class for representing a UI element
 *				that can be placed in UI hierarchy.
 *			Implements adapter pattern for cc::Node.
*/
#pragma once

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/String/String.h"

class Vector2;

namespace UI
{
	class ElementComponent;

	class Element : public cc::Sprite
	{
	public:
		static const String& GetType();

		static Element* Create();

		bool							GetScale(Vector2& outScale) const;
		bool							GetSize(Vector2& outSize) const;
		bool							GetPos(Vector2& outPos) const;
		bool							GetNormalizedPos(Vector2& outPos) const;
		const std::vector<Element*>		FindChildElements() const;

		void SetSize(const Vector2& size);
		void SetPos(const Vector2& pos);
		void SetScale(const Vector2& scale);
		void SetScaleX(float scaleX);
		void SetScaleY(float scaleY);
		void SetNormalizedPos(const Vector2& pos);
		void SetAnchorPos(const Vector2& pos);
		void SetImage(const String& imagePath);

		bool Init();
		void AddChild(Element& element);
		void AddComponent(ElementComponent& component);

	private:
		static String s_type;
	};
}
