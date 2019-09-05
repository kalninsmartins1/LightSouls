/*!
 * \Author	Martins Kalnins
 * \Date	20.07.2019
 * \Brief	Data container for ui element configuration.
*/
#pragma once

#include <string>
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class ElementConfig
	{
	public:
		ElementConfig();
		~ElementConfig();

		const Vector2&	GetAnchorPosition() const;
		const Vector2&	GetNormalizedPosition() const;
		const String&	GetPathToImage() const;
		const Vector2&	GetScale() const;

		void SetAnchorPosition(const Vector2& anchorPos);
		void SetNormalizedPosition(const Vector2& normalizedPos);
		void SetPathToImage(const String& pathToSprite);
		void SetScale(const Vector2& scale);

	private:
		String	m_pathToImage;
		Vector2	m_achorPosition;
		Vector2	m_normalizedPosition;
		Vector2	m_scale;
	};
}
