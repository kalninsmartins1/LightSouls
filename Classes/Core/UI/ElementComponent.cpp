#include "ElementComponent.h"

using namespace UI;

ElementComponent::ElementComponent(Element& owner)
	: m_owner(owner)
{

}

Element& UI::ElementComponent::GetOwner() const
{
	return m_owner;
}

bool UI::ElementComponent::Init()
{
	return cc::Component::init();
}

void UI::ElementComponent::Update(float deltaTime)
{

}

void UI::ElementComponent::update(float deltaTime)
{
	cc::Component::update(deltaTime);
	Update(deltaTime);
}
