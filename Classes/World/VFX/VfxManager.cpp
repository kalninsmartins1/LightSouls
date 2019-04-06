/* Class implementation details
 * Requirements:
 *  - Be generic data driven.
 *	- Have 1 VFX for every defined event
 *	- When an event is triggered spawn the VFX in container.
 * Solution:
 *	- Store events to VFX map
 *	- When event is triggered use the map to figure out which one to trigger.
 *	- Events also need to have information about the position for VFX to spawn at.
*/

#include "VFXManager.h"
#include "VFX.h"
#include "Classes/Events/PositionEventData.h"
#include "Classes/Utils/XML/XMLLoader.h"

VFXManager::VFXManager()
	: m_container(nullptr)
{

}

void VFXManager::AddVFX(const String& eventType, const String& pathToVfx)
{
	if (m_container != nullptr)
	{
		VFX* vfx = VFX::Create(*m_container, pathToVfx);
		m_eventToVFX[eventType] = vfx;

		// Register for this event
		cc::EventDispatcher* eventDispather = m_container->getEventDispatcher();
		if (eventDispather != nullptr)
		{
			eventDispather->addCustomEventListener(eventType,
				CC_CALLBACK_1(VFXManager::OnVFXEventTriggered, this));
		}
	}
}

void VFXManager::Cleanup()
{
	m_eventToVFX.clear();
}

bool VFXManager::Init(cc::Node* container, const String& pathToXML)
{
	m_container = container;
	return XMLLoader::InitializeVFXManager(*this, pathToXML);
}

void VFXManager::OnVFXEventTriggered(cc::EventCustom* eventData)
{
	if (eventData != nullptr)
	{
		PositionEventData* vfxEventData = static_cast<PositionEventData*>(eventData->getUserData());
		if (vfxEventData != nullptr)
		{
			const String& evenType = eventData->getEventName();
			VFX* vfx = m_eventToVFX[evenType];
			if (vfx != nullptr)
			{
				vfx->Spawn(vfxEventData->GetPosition());
			}
		}
	}	
}
