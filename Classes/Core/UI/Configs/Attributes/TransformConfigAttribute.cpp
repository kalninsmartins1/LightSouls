#include "TransformConfigAttribute.h"
#include "Classes/Core/UI/Element.h"

using namespace UI;

String TransformConfigAttribute::s_type = "TransformAttribute";

UI::TransformConfigAttribute::TransformConfigAttribute()
	: m_anchorPos(Vector2::GetZero())
	, m_normalizedPos(Vector2::GetZero())
	, m_scale(Vector2::GetZero())
{

}

const String& UI::TransformConfigAttribute::GetType()
{
	return s_type;
}

int UI::TransformConfigAttribute::GetPriority() const
{
	return 0;
}

const Vector2& TransformConfigAttribute::GetAnchorPosition() const
{
	return m_anchorPos;
}

const Vector2& TransformConfigAttribute::GetNormalizedPosition() const
{
	return m_normalizedPos;
}

const Vector2& TransformConfigAttribute::GetScale() const
{
	return m_scale;
}

std::unique_ptr<IConfigAttribute> UI::TransformConfigAttribute::Clone() const
{
	return std::unique_ptr<IConfigAttribute>(std::make_unique<TransformConfigAttribute>(*this));
}

void TransformConfigAttribute::SetAnchorPos(const Vector2& anchorPos)
{
	m_anchorPos = anchorPos;
}

void TransformConfigAttribute::SetNormalizedPos(const Vector2& normalizedPos)
{
	m_normalizedPos = normalizedPos;
}

void TransformConfigAttribute::SetScale(const Vector2& scale)
{
	m_scale = scale;
}

void UI::TransformConfigAttribute::Apply(Element& target) const
{
	target.SetScale(m_scale);
	target.SetAnchorPos(m_anchorPos);
	target.SetNormalizedPos(m_normalizedPos);
}
