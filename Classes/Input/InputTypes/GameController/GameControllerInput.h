#pragma once

#include "LightSoulsTypes.h"
#include "Input/InputTypes/AInputDevice.h"



using ButtonCode = cocos2d::Controller::Key;

struct ControllerAxis
{
	const int buttonCode;
	float curValue;
	float fromValue;
	float toValue;
	bool isPressed;

	ControllerAxis() :
		buttonCode(0)
	{
		curValue = 0;
		fromValue = 0;
		toValue = 0;
		isPressed = false;
	}

	ControllerAxis(int buttonCode, float fromValue, float toValue) :
		buttonCode(buttonCode)
	{
		this->curValue = 0;
		this->fromValue = fromValue;
		this->toValue = toValue;
		this->isPressed = false;
	}
};

enum class X360Button
{
	NONE = -1,
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LB = 4,
	RB = 5,
	BACK = 6,
	START = 7,
	LEFT_STICK = 8,
	RIGHT_STICK = 9,
	UP = 10,
	RIGHT = 11,
	DOWN = 12,
	LEFT = 13
};

enum class X360Axes
{
	NONE = -1,
	LEFT_STICK_X = 0,
	LEFT_STICK_Y = 1,
	RIGHT_STICK_X = 2,
	RIGHT_STICK_Y = 3,
	LT = 4,
	RT = 5
};

class GameControllerInput : public AInputDevice
{
public:
	GameControllerInput();
	~GameControllerInput();

public:	
	virtual bool	HasAxisInput(const String& axisName) const override;
	virtual float	GetAxisInput(const String& axisName) const override;
	bool			IsConnected() const;

	bool			Init();	
	void			AddAxisButton(const String& actionName, const ControllerAxis& axisButton);
	virtual void	ResetInputState() override;

private:	
	void SetAxisInputState(float value, int buttonCode);

	void OnControllerConnected();
	void OnControllerDisconnected();
	void OnButtonDown(cocos2d::Controller* pController, int buttonCode, cocos2d::Event* pEvent);
	void OnButtonUp(cocos2d::Controller* pController, int buttonCode, cocos2d::Event* pEvent);
	void OnAxisInput(cocos2d::Controller* pController, int axisCode, cocos2d::Event* pEvent);

private:
	static bool							s_isConnected;
	std::map<String, ControllerAxis>	m_controllerAxis;
	std::map<int, String>				m_buttonCodeToAxisAction;
};




