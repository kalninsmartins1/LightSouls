#include "AInputDevice.h"
#include "Utils/Utils.h"



void AInputDevice::Update(float deltaTime)
{
	UpdateActionButtonState();
}

void AInputDevice::SetActionButtonState(bool isActive, int inputCode)
{	
	if (Utils::ContainsKey(m_buttonCodeToAction, inputCode))
	{
		String& actionName = m_buttonCodeToAction[inputCode];
		m_actionButtons[actionName].isActive = isActive;
	}
}

void AInputDevice::SetStateButtonState(bool isPressed, int inputCode)
{
	/*
	* If the key is found then change its
	* state to specified on.
	*/
	if (Utils::ContainsKey(m_buttonCodeToStateAction, inputCode))
	{
		String& actionName = m_buttonCodeToStateAction[inputCode];
		m_stateButtons[actionName].isPressed = isPressed;
	}
}

void AInputDevice::UpdateActionButtonState()
{
	// Make sure action buttons are only active for one frame
	for (auto& pair : m_actionButtons)
	{
		ActionButton& key = pair.second;
		if (key.isActive && !key.needStateReset)
		{
			key.needStateReset = true;
		}
		else if (key.needStateReset)
		{
			key.isActive = false;
			key.needStateReset = false;
		}
	}
}

void AInputDevice::AddStateButton(const String& actionName, const StateButton& stateButton)
{
	m_stateButtons.insert(std::make_pair(actionName, stateButton));
	m_buttonCodeToStateAction[stateButton.buttonCode] = actionName;
}

void AInputDevice::ResetInputState()
{
	// Clear all the input data
	for (auto& stateButton : m_stateButtons)
	{
		stateButton.second.isPressed = false;
	}
	
	for (auto& actionButton : m_actionButtons)
	{		
		actionButton.second.isActive = false;
	}	
}

bool AInputDevice::HasAction(const String& action) const
{
	bool isActive = false;
	if (Utils::ContainsKey(m_actionButtons, action))
	{
		isActive = m_actionButtons.at(action).isActive;
	}
	return isActive;
}

bool AInputDevice::HasActionState(const String& action) const
{
	bool isActiveState = false;
	if (Utils::ContainsKey(m_stateButtons, action))
	{
		isActiveState = m_stateButtons.at(action).isPressed;
	}
	return isActiveState;
}

void AInputDevice::AddActionButton(const String& actionName, const ActionButton& actionButton)
{
	m_actionButtons.insert(std::make_pair(actionName, actionButton));
	m_buttonCodeToAction[actionButton.buttonCode] = actionName;
}


