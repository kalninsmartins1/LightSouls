#include "AInputDevice.h"

void AInputDevice::update(float deltaTime)
{
	updateActionButtonState();
}

void AInputDevice::updateActionButtonState()
{
	// Make sure action buttons are only active for one frame
	for (auto& pair : m_actionButtons)
	{
		ActionButton& key = pair.second;
		if (key.bIsActive && !key.bNeedStateReset)
		{
			key.bNeedStateReset = true;
		}
		else if (key.bNeedStateReset)
		{
			key.bIsActive = false;
			key.bNeedStateReset = false;
		}
	}
}
