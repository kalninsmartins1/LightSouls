/*!
 * \Author	Martins Kalnins
 * \Date	20.09.2019
 * \Brief	UI element config attribute for
 *				making element a progress bar.
*/
#pragma once

#include "IConfigAttribute.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class ProgressBarConfigAttribute : public IConfigAttribute
	{
	public:
		static const String& GetType();

		virtual int									GetPriority() const override;
		virtual std::unique_ptr<IConfigAttribute>	Clone() const override;

		void SetAnimSpeed(float speed);
		void SetEventName(const String& eventName);

		virtual void Apply(Element& target) const override;

	private:
		static String s_type;

		float	m_animSpeed;
		String	m_eventName;
	};
}