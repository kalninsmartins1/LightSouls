#pragma once

#include <string>
#include <map>


struct ActionButton
{
	const int buttonCode;
	bool bIsActive;
	bool bNeedStateReset;
	
	ActionButton() :
		buttonCode(0)
	{
		bIsActive = false;
		bNeedStateReset = false;
	}

	ActionButton(int buttonCode) :
		buttonCode(buttonCode)
	{
		bIsActive = false;
		bNeedStateReset = false;
	}
};

struct StateButton
{
	const int buttonCode;
	bool bIsPressed;

	StateButton() :
		buttonCode(0)
	{
		bIsPressed = false;
	}

	StateButton(int buttonCode) :
		buttonCode(buttonCode)
	{
		bIsPressed = false;
	}
};

class AInputDevice
{
public:

	// Returns true for action once when input is released
	virtual bool hasAction(const std::string& action) const = 0;

	// Returns true while input for action is not released
	virtual bool hasActionState(const std::string& action) const = 0;

	// Returns true if this input device has specified axis input
	virtual bool hasAxisInput(const std::string& axisName) const = 0;

	// Returs current axis input value normalized in range 0 to 1
	virtual float getAxisInput(const std::string& axisName) const = 0;
	
	// Binds action button to specified action name
	virtual void addActionButton(const char* actionName, const ActionButton& actionButton) = 0;

	// Binds state button to specified action name
	virtual void addStateButton(const char* actionName, const StateButton& stateButton) = 0;

	// Manages consistency between actions and buttons
	virtual void update(float deltaTime);

protected:
	void updateActionButtonState();
	
	std::map<std::string, StateButton> m_stateButtons;
	std::map<std::string, ActionButton> m_actionButtons;
	
	std::map<int, std::string> m_buttonCodeToAction;
	std::map<int, std::string> m_buttonCodeToStateAction;
	
};