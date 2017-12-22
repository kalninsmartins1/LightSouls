#pragma once

#include "cocos2d.h"
#include "Input/InputTypes/IInputDevice.h"
#include "Utils/Utils.h"

struct StateMouseButton
{
	std::string action;
	bool bIsPressed;

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

class MouseInput: cocos2d::Ref, IInputDevice
{
public:	
	CREATE_FUNC(MouseInput);

	bool init();
	void update(float deltaTime);
	void addButtonAction(const char* buttonAction, const char* buttonCode);
	void addButtonState(const char* buttonAction, const char* buttonCode);

	virtual bool HasAction(const std::string& action) const override;
	virtual bool HasActionState(const std::string& action) const override;
private:
	void onMouseButtonDown(cocos2d::EventMouse* pEvent);
	void onMouseButtonUp(cocos2d::EventMouse* pEvent);

	template <typename T, typename K, typename L, typename P>
	void switchButtonState(std::map<T,K>& stateMap, std::map<L, P>& codeMap,
		MouseButtonCode buttonCode, bool newState);

	ActionStateMap m_stateButtons;
	ActionStateMap m_actionButtons;
	StateButtonCodeMap m_buttonStates;
	ActionButtonCodeMap m_buttonActions;
};

template <typename T, typename K, typename L, typename P>
void MouseInput::switchButtonState(std::map<T, K>& stateMap,
	std::map<L, P>& codeMap, MouseButtonCode buttonCode, bool newState)
{
	if (Utils::containsKey(codeMap, buttonCode))
	{
		StateMouseButton& button = codeMap[buttonCode];
		button.bIsPressed = newState;
		stateMap[button.action] = button.bIsPressed;
	}
}
