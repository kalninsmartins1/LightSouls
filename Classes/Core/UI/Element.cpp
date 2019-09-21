#include "Element.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/UI/ElementComponent.h"

using namespace UI;

String Element::s_type = "Element";

const String& UI::Element::GetType()
{
	return s_type;
}

Element* UI::Element::Create()
{
	Element* elem = new (std::nothrow) Element();
	if (elem != nullptr && elem->Init())
	{
		elem->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(elem);
	}

	return elem;
}

bool UI::Element::GetScale(Vector2& outScale) const
{
	outScale.Set(getScaleX(), getScaleY());
	return true;
}

bool UI::Element::GetSize(Vector2& outSize) const
{
	auto size = getContentSize();
	outSize.Set(size.width, size.height);
	return true;
}

bool Element::GetPos(Vector2& outPos) const
{
	outPos.Set(getPositionX(), getPositionY());
	return true;
}

bool UI::Element::GetNormalizedPos(Vector2& outPos) const
{
	auto pos = getNormalizedPosition();
	outPos.Set(pos.x, pos.y);
	return true;
}

const std::vector<Element*> UI::Element::FindChildElements() const
{
	std::vector<Element*> childElements;
	for (auto& child : getChildren())
	{
		if (child != nullptr)
		{
			childElements.emplace_back(static_cast<Element*>(child));
		}
	}

	return childElements;
}

void UI::Element::SetSize(const Vector2& size)
{
	setContentSize(cc::Size(size.GetX(), size.GetY()));
}

void Element::SetPos(const Vector2& pos)
{
	setPosition(pos.GetX(), pos.GetY());
}

void UI::Element::SetScale(const Vector2& scale)
{
	setScale(scale.GetX(), scale.GetY());
}

void UI::Element::SetScaleX(float scaleX)
{
	setScaleX(scaleX);
}

void UI::Element::SetScaleY(float scaleY)
{
	setScaleY(scaleY);
}

void UI::Element::SetNormalizedPos(const Vector2& pos)
{
	setNormalizedPosition(cc::Vec2(pos.GetX(), pos.GetY()));
}

void UI::Element::SetAnchorPos(const Vector2& pos)
{
	setAnchorPoint(cc::Vec2(pos.GetX(), pos.GetY()));
}

void UI::Element::SetImage(const String& imagePath)
{
	initWithFile(imagePath.GetCStr());
}

bool UI::Element::Init()
{
	return cc::Sprite::init();
}

void UI::Element::AddChild(Element& element)
{
	addChild(&element);
}

void UI::Element::AddComponent(ElementComponent& component)
{
	addComponent(&component);
}
