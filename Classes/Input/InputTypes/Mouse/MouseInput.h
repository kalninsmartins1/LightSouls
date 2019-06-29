#pragma once

#include "LightSoulsTypes.h"
#include "Input/InputTypes/AInputDevice.h"
#include "Utils/Utils.h"



using MouseButtonCode = cocos2d::EventMouse::MouseButton;

enum class MouseAxisType : char
{
	NONE,
	X,
	Y
};

struct MouseAxis
{
	MouseAxisType type;
	float         valueFrom;
	float         valueTo;
	float		  curValue;

	MouseAxis()
		: type(MouseAxisType::NONE)
		, valueFrom(0.0f)
		, valueTo(0.0f)
		, curValue(0.0f)
	{

	}

	MouseAxis(MouseAxisType type, float valueFrom, float valueTo)
		: type(type)
		, valueFrom(valueFrom)
		, valueTo(valueTo)
		, curValue(0.0f)
	{

	}
};

class MouseInput: public AInputDevice
{
public:
	MouseInput();

public:	
	virtual bool	HasAxisInput(const String& axisName) const override;
	virtual float	GetAxisInput(const String& axisName) const override;

	void			AddAxisAction(const String& actionName, const MouseAxis& axis);
	bool			Init();
	
private:
	void SetActionButtonState(bool isActive, const MouseButtonCode& keyCode);
	void SetStateButtonState(bool isPressed, const MouseButtonCode& keyCode);
	
	void UpdateMouseAxis(const Vector2& moveDirection);
	void OnMouseButtonDown(cc::EventMouse* eventData);
	void OnMouseButtonUp(cc::EventMouse* eventData);
	void OnMouseMoved(cc::EventMouse* eventData);

private:
	std::map<String, MouseAxis> m_mouseAxis;
};


