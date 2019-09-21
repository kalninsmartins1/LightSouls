/*!
 * \Author	Martins Kalnins
 * \Date	17.09.2019
 * \Brief	Common interface for all UI
 *			 element attributes.
*/
#pragma once

#include "IConfigAttribute.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class TransformConfigAttribute : public IConfigAttribute
	{
	public:
		TransformConfigAttribute();

		static const String& GetType();

		virtual int									GetPriority() const override;
		const Vector2&								GetAnchorPosition() const;
		const Vector2&								GetNormalizedPosition() const;
		const Vector2&								GetScale() const;
		virtual std::unique_ptr<IConfigAttribute>	Clone() const override;

		void SetAnchorPos(const Vector2& anchorPos);
		void SetNormalizedPos(const Vector2& normalizedPos);
		void SetScale(const Vector2& scale);

		virtual void Apply(Element& target) const override;

	private:
		static String s_type;

		Vector2 m_anchorPos;
		Vector2 m_normalizedPos;
		Vector2 m_scale;
	};
}
