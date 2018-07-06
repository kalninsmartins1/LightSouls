#pragma once

#include "LightSoulsTypes.h"
#include "Input/InputTypes/AInputDevice.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

using MouseButtonCode = cocos2d::EventMouse::MouseButton;

class MouseInput: public AInputDevice
{
public:
	MouseInput();

public:	
	virtual bool	HasAxisInput(const String& axisName) const override;
	virtual float	GetAxisInput(const String& axisName) const override;

	bool			Init();
	
private:
	void SetActionButtonState(bool isActive, const MouseButtonCode& keyCode);
	void SetStateButtonState(bool isPressed, const MouseButtonCode& keyCode);
	
	void OnMouseButtonDown(cocos2d::EventMouse* pEvent);
	void OnMouseButtonUp(cocos2d::EventMouse* pEvent);	
};

NS_LIGHTSOULS_END
