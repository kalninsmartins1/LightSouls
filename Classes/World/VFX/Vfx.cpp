#include "Vfx.h"
#include "Classes/Utils/XML/XMLLoader.h"
#include "Classes/GameConsts.h"
#include "Classes/World/Entity/Components/AnimComponent.h"

Vfx::Vfx(cc::Node& container)
	: m_container(container)
{

}

Vfx* Vfx::Create(cc::Node& container, const String& pathToXML)
{
	Vfx* vfx = new (std::nothrow) Vfx();
	if (vfx != nullptr && vfx->Init(container, pathToXML))
	{
		vfx->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(vfx);
	}

	return vfx;
}

void Vfx::Spawn(cc::Node& container, const Vector2& pos)
{
	// Only spawn if has a animation component	
	if (m_animComp != nullptr)
	{
		setVisible(true);
		setPosition(pos);
		m_animComp->PlayOneShotAnimation(GameConsts::ANIM_TYPE_ACT,
			CC_CALLBACK_0(Vfx::OnFinishedAnimating, this));
	}	
}

bool Vfx::Init(cc::Node& container, const String& pathToXML)
{
	bool isSuccessful = XMLLoader::InitializeComponents(*this, pathToXML);
	m_animComp = static_cast<AnimComponent*>(getComponent(GameConsts::ANIM_COMPONENT));
	isSuccessful &= m_animComp != nullptr;
	CCASSERT(isSuccessful, "Error could not find animation component !");

	if (isSuccessful)
	{
		container.addChild(this);
		setVisible(false);
	}

	return isSuccessful;
}

void Vfx::OnFinishedAnimating()
{
	setVisible(false);
}

