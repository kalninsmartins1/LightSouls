#pragma once

#include "Input/InputTypes/AInputDevice.h"
#include "cocos2d.h"

typedef cocos2d::Controller::Key ButtonCode;

struct ControllerAxis
{
	const int buttonCode;
	float curValue;
	float fromValue;
	float toValue;
	bool bIsPressed;

	ControllerAxis() :
		buttonCode(0)
	{
		curValue = 0;
		fromValue = 0;
		toValue = 0;
		bIsPressed = false;
	}

	ControllerAxis(int buttonCode, float fromValue, float toValue) :
		buttonCode(buttonCode)
	{
		this->curValue = 0;
		this->fromValue = fromValue;
		this->toValue = toValue;
		this->bIsPressed = false;
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
	bool init();

	virtual bool hasAction(const std::string& action) const override;
	virtual bool hasActionState(const std::string& action) const override;
	virtual bool hasAxisInput(const std::string& axisName) const override;
	virtual float getAxisInput(const std::string& axisName) const override;

	virtual void addActionButton(const char* actionName, const ActionButton& actionButton) override;
	virtual void addStateButton(const char* actionName, const StateButton& stateButton) override;

	void addAxisButton(const char* actionName, const ControllerAxis& axisButton);
	bool isConnected() const;

private:
	void onControllerConnected();
	void onControllerDisconnected();
	void onButtonDown(cocos2d::Controller* pController, int buttonCode,
		cocos2d::Event* pEvent);
	void onButtonUp(cocos2d::Controller* pController, int buttonCode,
		cocos2d::Event* pEvent);
	void onAxisInput(cocos2d::Controller* pController, int axisCode,
		cocos2d::Event* pEvent);

	void setActionButtonState(bool bIsActive, int buttonCode);
	void setStateButtonState(bool bIsPressed, int buttonCode);
	void setAxisInputState(float value, int buttonCode);

	bool m_bIsConnected;

	std::map<std::string, ControllerAxis> m_controllerAxis;
	std::map<int, std::string> m_buttonCodeToAxisAction;
};



