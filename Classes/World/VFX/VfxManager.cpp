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
		m_eventToVFXPath[eventType] = pathToVfx;
		// Register for this event
		cc::EventDispatcher* eventDispather = m_container->getEventDispatcher();
		if (eventDispather != nullptr)
		{
			eventDispather->addCustomEventListener(eventType,
				CC_CALLBACK_1(VFXManager::OnVFXEventTriggered, this));
		}
	}
}

bool VFXManager::Init(cc::Node* container, const String& pathToXML)
{
	// Clear any previous data
	m_eventToVFXPath.clear();

	// Get new data
	m_container = container;
	return XMLLoader::InitializeVFXManager(*this, pathToXML);
}

void VFXManager::OnVFXEventTriggered(cc::EventCustom* eventData)
{
	if (m_container != nullptr && eventData != nullptr)
	{
		PositionEventData* vfxEventData = static_cast<PositionEventData*>(eventData->getUserData());
		if (vfxEventData != nullptr)
		{
			const String& evenType = eventData->getEventName();
			const String& vfxInitPath = m_eventToVFXPath[evenType];
			VFX* vfx = VFX::Create(*m_container, vfxInitPath);

			if (vfx != nullptr)
			{
				vfx->Spawn(vfxEventData->GetPosition(), 
					CC_CALLBACK_1(VFXManager::OnVFXFinishedCallback, this));
			}
		}
	}	
}

void VFXManager::OnVFXFinishedCallback(VFX& vfx)
{
	if (m_container != nullptr)
	{
		m_container->removeChild(&vfx);
	}
}
