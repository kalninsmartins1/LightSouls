#include "VFX.h"
#include "Classes/Utils/XML/XMLLoader.h"
#include "Classes/GameConsts.h"
#include "Classes/World/Entity/Components/AnimComponent.h"

VFX::VFX()
	: m_animComp(nullptr)
{

}

VFX* VFX::Create(cc::Node& container, const String& pathToXML)
{
	VFX* vfx = new (std::nothrow) VFX();
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

void VFX::Spawn(const Vector2& pos, const std::function<void(VFX&)>& onFinished)
{	
	// Only spawn if has a animation component	
	if (m_animComp != nullptr)
	{
		setVisible(true);
		setPosition(pos);
		m_animComp->PlayOneShotAnimation(GameConsts::ANIM_TYPE_IDLE,
			CC_CALLBACK_0(VFX::OnFinishedAnimating, this));
		m_finishCallback = onFinished;
	}
	else
	{
		onFinished(*this);
	}
}

bool VFX::Init(cc::Node& container, const String& pathToXML)
{
	bool isSuccessful = XMLLoader::InitializeComponents(*this, pathToXML);
	m_animComp = static_cast<AnimComponent*>(getComponent(GameConsts::ANIM_COMPONENT));
	isSuccessful &= m_animComp != nullptr;
	CCASSERT(isSuccessful, "Error could not find animation component !");

	if (isSuccessful)
	{
		setCameraMask(container.getCameraMask());
		container.addChild(this);
		setVisible(false);
	}

	return isSuccessful;
}

void VFX::OnFinishedAnimating()
{
	setVisible(false);
	m_finishCallback(*this);
}

