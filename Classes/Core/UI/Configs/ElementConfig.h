/*!
 * \Author	Martins Kalnins
 * \Date	20.07.2019
 * \Brief	Data container for ui element configuration.
*/
#pragma once

#include <memory>
#include <vector>
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"
#include "Classes/Core/UI/Configs/Attributes/IConfigAttribute.h"

namespace UI
{
	class Element;

	class ElementConfig
	{
	public:
		ElementConfig() = default;
		ElementConfig(const ElementConfig& other);
		ElementConfig& operator=(const ElementConfig& other);

		const String&		GetType() const;
		virtual Element*	Create() const;

		void SetType(const String& type);

		void			AddAttribute(const IConfigAttribute& attribute);
		virtual void	Configure(Element& element) const;

	private:
		static bool CompareAttributes(const std::unique_ptr<IConfigAttribute>& attrA, const std::unique_ptr<IConfigAttribute>& attrB);

		void DeepCopyAttributes(const std::vector<std::unique_ptr<IConfigAttribute>>& attributes);

	private:
		String											m_type;
		std::vector<std::unique_ptr<IConfigAttribute>>	m_attributes;
	};
}
