#pragma once

#include <map>

struct ActionButton
{
	const int buttonCode;
	bool isActive;
	bool needStateReset;
	
	ActionButton() :
		buttonCode(0)
	{
		isActive = false;
		needStateReset = false;
	}

	ActionButton(int buttonCode) :
		buttonCode(buttonCode)
	{
		isActive = false;
		needStateReset = false;
	}
};

struct StateButton
{
	const int buttonCode;
	bool isPressed;

	StateButton() :
		buttonCode(0)
	{
		isPressed = false;
	}

	StateButton(int buttonCode) :
		buttonCode(buttonCode)
	{
		isPressed = false;
	}
};

class String;

class AInputDevice
{
public:
	// Returns true for action once when input is released
	bool	HasAction(const String& action) const;

	// Returns true while input for action is not released
	bool	HasActionState(const String& action) const;

	// Returns true if this input device has specified axis input
	virtual bool	HasAxisInput(const String& axisName) const = 0;

	// Returs current axis input value normalized in range 0 to 1
	virtual float	GetAxisInput(const String& axisName) const = 0;
	
	// Manages consistency between actions and buttons
	virtual void	Update(float deltaTime);

	// Binds action button to specified action name
	void			AddActionButton(const String& actionName, const ActionButton& actionButton);

	// Binds state button to specified action name
	void			AddStateButton(const String& actionName, const StateButton& stateButton);

	virtual void	ResetInputState();

protected:
	void SetActionButtonState(bool isActive, int inputCode);
	void SetStateButtonState(bool isPressed, int inputCode);

	void UpdateActionButtonState();	

private:
	std::map<String, StateButton>	m_stateButtons;
	std::map<String, ActionButton>	m_actionButtons;
	
	std::map<int, String> m_buttonCodeToAction;
	std::map<int, String> m_buttonCodeToStateAction;	
};


