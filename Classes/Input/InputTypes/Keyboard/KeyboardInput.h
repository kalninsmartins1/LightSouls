#pragma once

#include <map>
#include "cocos2d.h"
#include "Input/InputTypes/IInputDevice.h"

typedef cocos2d::EventKeyboard::KeyCode KeyCode;

struct KeyboardAxis
{
	KeyCode keyCodeFrom;
	KeyCode keyCodeTo;

	float fromValue;
	float toValue;
	float currentValue;

	bool bFromIsPressed;
	bool bToIsPressed;
	
	KeyboardAxis()
	{
		this->keyCodeFrom = KeyCode::KEY_NONE;
		this->keyCodeTo = KeyCode::KEY_NONE;
		this->fromValue = 0;
		this->toValue = 0;
		this->currentValue = 0;
		this->bFromIsPressed = false;
		this->bToIsPressed = false;
	}

	KeyboardAxis(KeyCode keyCodeFrom, KeyCode keyCodeTo, float fromValue, float toValue)
	{
		this->keyCodeFrom = keyCodeFrom;
		this->keyCodeTo = keyCodeTo;
		this->fromValue = fromValue;
 		this->toValue = toValue;
		this->currentValue = 0;
		this->bFromIsPressed = false;
		this->bToIsPressed = false;
	}
};

struct StateKey
{
	KeyCode keyCode;
	bool bIsCurrentlyPressed;

	StateKey()
	{
		this->keyCode = KeyCode::KEY_NONE;
		this->bIsCurrentlyPressed = false;
	}

	StateKey(KeyCode keyCode)
	{
		this->keyCode = keyCode;
		this->bIsCurrentlyPressed = false;
	}
};

struct ActionKey
{
	KeyCode keyCode;
	bool bIsActive;
	bool bNeedsStateReset;

	ActionKey()
	{
		this->keyCode = KeyCode::KEY_NONE;
		this->bIsActive = false;
		this->bNeedsStateReset = false;
	}

	ActionKey(KeyCode keyCode)
	{
		this->keyCode = keyCode;
		this->bIsActive = false;
		this->bNeedsStateReset = false;
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

	void addAxisKey(const char* actionName, const KeyboardAxis& axisKey);
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
	void increaseAxisCurValue(KeyboardAxis& keyboardAxis, float value, float deltaTime);

	void setActionKeyState(bool bIsActive, KeyCode keyCode);
	void setStateKeyState(bool bIsPressed, KeyCode keyCode);
	void setKeyboardAxisState(bool bIsPressed, KeyCode keyCode);

	std::map<std::string, KeyboardAxis> m_keyboardAxis;
	std::map<std::string, StateKey> m_stateKeys;
	std::map<std::string, ActionKey> m_actionKeys;
	
	std::map<KeyCode, std::string> m_keyCodeToAction;	
	std::map<KeyCode, std::string> m_keyCodeToStateAction;	
	std::map<KeyCode, std::string> m_keyCodeToAxisAction;	
};
