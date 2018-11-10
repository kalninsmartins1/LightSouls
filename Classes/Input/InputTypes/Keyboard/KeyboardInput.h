#pragma once

#include <map>
#include "Input/InputTypes/AInputDevice.h"



using KeyCode = cocos2d::EventKeyboard::KeyCode;

struct KeyboardAxis
{
	const KeyCode keyCodeFrom;
	const KeyCode keyCodeTo;

	float fromValue;
	float toValue;
	float currentValue;

	bool fromIsPressed;
	bool toIsPressed;
	
	KeyboardAxis() :
		keyCodeFrom(KeyCode::KEY_NONE),
		keyCodeTo(KeyCode::KEY_NONE)
	{
		this->fromValue = 0;
		this->toValue = 0;
		this->currentValue = 0;
		this->fromIsPressed = false;
		this->toIsPressed = false;
	}

	KeyboardAxis(KeyCode keyCodeFrom, KeyCode keyCodeTo, float fromValue, float toValue) :
		keyCodeFrom(keyCodeFrom),
		keyCodeTo(keyCodeTo)
	{
		this->fromValue = fromValue;
 		this->toValue = toValue;
		this->currentValue = 0;
		this->fromIsPressed = false;
		this->toIsPressed = false;
	}
};

class KeyboardInput: public AInputDevice
{
public:
	KeyboardInput();

public:
	bool			Init();

	virtual bool	HasAxisInput(const String& axisName) const override;
	virtual float	GetAxisInput(const String& axisName) const override;
		
	void			AddKeyboardAxis(const String& actionName, const KeyboardAxis& axisKey);
	virtual void	Update(float deltaTime) override;	
	virtual void	ResetInputState() override;

private:
	void SetActionKeyState(bool isActive, KeyCode keyCode);
	void SetStateKeyState(bool isPressed, KeyCode keyCode);
	void SetKeyboardAxisState(bool isPressed, KeyCode keyCode);

	static void IncreaseAxisCurValue(KeyboardAxis& keyboardAxis, float value, float deltaTime);
	void		OnKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, 
					cocos2d::Event* pEvent);
	void		OnKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, 
					cocos2d::Event* pEvent);		
	void		UpdateAxisKeyState(float deltaTime);
	
private:
	std::map<String, KeyboardAxis> m_keyboardAxis;
	std::map<KeyCode, String> m_keyCodeToAxisAction;	
};


