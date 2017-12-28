#pragma once

#include <map>
#include "cocos2d.h"
#include "Input/InputTypes/IInputDevice.h"

typedef cocos2d::EventKeyboard::KeyCode KeyCode;

struct AxisKey
{
	KeyCode keyCode;
	float minValue;
	float maxValue;
	float currentValue;
	bool isPressed;
	
	AxisKey(KeyCode keyCode, float minValue, float maxValue)
	{
		this->keyCode = keyCode;		
		this->minValue = minValue;
		this->maxValue = maxValue;
		this->currentValue = 0;
	}
};

struct StateKey
{
	KeyCode keyCode;
	bool isCurrentlyPressed;

	StateKey(KeyCode keyCode)
	{
		this->keyCode = keyCode;
		this->isCurrentlyPressed = false;
	}
};

struct ActionKey
{
	KeyCode keyCode;
	bool isActive;
	bool needsStateReset;

	ActionKey(KeyCode keyCode)
	{
		this->keyCode = keyCode;
		this->isActive = false;
		this->needsStateReset = false;
	}
};

class KeyboardInput: IInputDevice
{
public:
	KeyboardInput();
	bool init();

	virtual bool hasAction(const std::string& action) const override;
	virtual bool hasActionState(const std::string& action) const override;
	virtual float getAxisInput(const std::string& axisName) const override;
	virtual bool hasAxisInput(const std::string& axisName) const override;

	void addAxisKey(const char* actionName, const AxisKey& axisKey);
	void addActionKey(const char* actionName, const ActionKey& actionKey);
	void addStateKey(const char* actionName, const StateKey& stateKey);

	void update(float deltaTime);
	
private:
	void onKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
	void onKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);

	void updateActionKeyState();
	void updateAxisKeyState(float deltaTime);

	void setActionKeyState(bool isActive, KeyCode keyCode);
	void setStateKeyState(bool isPressed, KeyCode keyCode);
	void setAxisKeyValue(bool isPressed, KeyCode keyCode);

	std::map<std::string, AxisKey> m_axisKeys;
	std::map<std::string, StateKey> m_stateKeys;
	std::map<std::string, ActionKey> m_actionKeys;
	
	std::map<KeyCode, std::string> m_keyCodeToAction;	
	std::map<KeyCode, std::string> m_keyCodeToState;	
	std::map<KeyCode, std::string> m_keyCodeToAxis;	
};
