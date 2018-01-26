#pragma once

#include <map>
#include "cocos2d.h"
#include "Input/InputTypes/AInputDevice.h"

typedef cocos2d::EventKeyboard::KeyCode KeyCode;

struct KeyboardAxis
{
	const KeyCode keyCodeFrom;
	const KeyCode keyCodeTo;

	float fromValue;
	float toValue;
	float currentValue;

	bool bFromIsPressed;
	bool bToIsPressed;
	
	KeyboardAxis() :
		keyCodeFrom(KeyCode::KEY_NONE),
		keyCodeTo(KeyCode::KEY_NONE)
	{
		this->fromValue = 0;
		this->toValue = 0;
		this->currentValue = 0;
		this->bFromIsPressed = false;
		this->bToIsPressed = false;
	}

	KeyboardAxis(KeyCode keyCodeFrom, KeyCode keyCodeTo, float fromValue, float toValue) :
		keyCodeFrom(keyCodeFrom),
		keyCodeTo(keyCodeTo)
	{
		this->fromValue = fromValue;
 		this->toValue = toValue;
		this->currentValue = 0;
		this->bFromIsPressed = false;
		this->bToIsPressed = false;
	}
};

class KeyboardInput: public AInputDevice
{
public:
	KeyboardInput();
	bool init();

	virtual bool hasAction(const std::string& action) const override;
	virtual bool hasActionState(const std::string& action) const override;
	virtual bool hasAxisInput(const std::string& axisName) const override;
	virtual float getAxisInput(const std::string& axisName) const override;
	
	virtual void addActionButton(const std::string& actionName, const ActionButton& actionKey) override;
	virtual void addStateButton(const std::string& actionName, const StateButton& stateKey) override;
	virtual void update(float deltaTime) override;
	
	void addKeyboardAxis(const std::string& actionName, const KeyboardAxis& axisKey);

private:
	static void increaseAxisCurValue(KeyboardAxis& keyboardAxis, float value, float deltaTime);

	void onKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
	void onKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
		
	void updateAxisKeyState(float deltaTime);
	
	void setActionKeyState(bool bIsActive, KeyCode keyCode);
	void setStateKeyState(bool bIsPressed, KeyCode keyCode);
	void setKeyboardAxisState(bool bIsPressed, KeyCode keyCode);

	std::map<std::string, KeyboardAxis> m_keyboardAxis;
	std::map<KeyCode, std::string> m_keyCodeToAxisAction;	
};
