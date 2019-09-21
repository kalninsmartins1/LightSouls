#include "ElementConfig.h"
#include "Classes/Utils/XML/XMLTypes.h"
#include "Classes/Utils/XML/XMLLoader.h"
#include "Classes/Utils/XML/XMLConsts.h"
#include "Classes/Core/UI/Element.h"

using namespace UI;

UI::ElementConfig::ElementConfig(const ElementConfig& other)
	: m_type(other.m_type)
{
	DeepCopyAttributes(other.m_attributes);
}

ElementConfig& ElementConfig::operator=(const ElementConfig& other)
{
	m_type = other.GetType();
	DeepCopyAttributes(other.m_attributes);
	return *this;
}

const String& UI::ElementConfig::GetType() const
{
	return m_type;
}

UI::Element* UI::ElementConfig::Create() const
{
	auto elem = Element::Create();
	if (elem != nullptr)
	{
		Configure(*elem);
	}

	return elem;
}

void UI::ElementConfig::SetType(const String& type)
{
	m_type = type;
}

void UI::ElementConfig::Configure(Element& element) const
{
	for (auto& attr : m_attributes)
	{
		if (attr != nullptr)
		{
			attr->Apply(element);
		}
	}
}

bool UI::ElementConfig::CompareAttributes(const std::unique_ptr<IConfigAttribute>& attrA, const std::unique_ptr<IConfigAttribute>& attrB)
{
	bool isHigher = false;
	if (attrA != nullptr && attrB != nullptr)
	{
		isHigher = attrA->GetPriority() > attrB->GetPriority();
	}

	return isHigher;
}

void UI::ElementConfig::DeepCopyAttributes(const std::vector<std::unique_ptr<IConfigAttribute>>& attributes)
{
	// Deep copy attributes
	for (auto& attr : attributes)
	{
		if (attr.get() != nullptr)
		{
			AddAttribute(*attr);
		}
	}
}

void UI::ElementConfig::AddAttribute(const IConfigAttribute& attribute)
{
	m_attributes.emplace_back(attribute.Clone());

	// Ensure vector stays sorted
	sort(m_attributes.begin(), m_attributes.end(), CompareAttributes);
}
