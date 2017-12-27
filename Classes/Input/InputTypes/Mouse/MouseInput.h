#pragma once

#include "cocos2d.h"
#include "Input/InputTypes/IInputDevice.h"
#include "Utils/Utils.h"

struct StateMouseButton
{
	std::string action;
	bool bIsPressed;

	StateMouseButton()
	{
		this->action = "";
		this->bIsPressed = false;
	}

	StateMouseButton(std::string action)
	{
		this->action = action;
		this->bIsPressed = false;
	}
};

struct ActionMouseButton
{
	std::string action;
	bool bIsActive;
	bool bNeedsStateReset;

	ActionMouseButton()
	{
		this->action = "";
		this->bIsActive = false;
		this->bNeedsStateReset = false;
	}

	ActionMouseButton(std::string action)
	{
		this->action = action;
		this->bIsActive = false;
		this->bNeedsStateReset = false;
	}
};

typedef cocos2d::EventMouse::MouseButton MouseButtonCode;
typedef std::map<std::string, bool> ActionStateMap;
typedef std::map<MouseButtonCode, StateMouseButton> StateButtonCodeMap;
typedef std::map<MouseButtonCode, ActionMouseButton> ActionButtonCodeMap;

class MouseInput: IInputDevice
{
public:		
	MouseInput();

	bool init();
	void update(float deltaTime);
	void addButtonAction(const char* buttonAction, const char* buttonCode);
	void addButtonState(const char* buttonAction, const char* buttonCode);

	virtual bool hasAction(const std::string& action) const override;
	virtual bool hasActionState(const std::string& action) const override;
private:
	void onMouseButtonDown(cocos2d::EventMouse* pEvent);
	void onMouseButtonUp(cocos2d::EventMouse* pEvent);
	void updateActionButtonState();
	
	void switchActionButtonState(ActionStateMap& stateMap, ActionButtonCodeMap& codeMap,
		MouseButtonCode buttonCode, bool newState);
	void switchStateButtonState(ActionStateMap& stateMap, StateButtonCodeMap& codeMap,
		MouseButtonCode buttonCode, bool newState);

	ActionStateMap m_stateButtons;
	ActionStateMap m_actionButtons;
	StateButtonCodeMap m_buttonStates;
	ActionButtonCodeMap m_buttonActions;
};
