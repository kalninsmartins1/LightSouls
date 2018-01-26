#pragma once

#include "cocos2d.h"
#include "Input/InputTypes/AInputDevice.h"
#include "Utils/Utils.h"

typedef cocos2d::EventMouse::MouseButton MouseButtonCode;

class MouseInput: public AInputDevice
{
public:
	MouseInput();
	bool init();

	virtual void addActionButton(const std::string& actionName, const ActionButton& actionButton) override;
	virtual void addStateButton(const std::string& actionName, const StateButton& stateButton) override;

	virtual bool hasAction(const std::string& action) const override;
	virtual bool hasActionState(const std::string& action) const override;
	virtual bool hasAxisInput(const std::string& axisName) const override;
	virtual float getAxisInput(const std::string& axisName) const override;
	
private:
	void onMouseButtonDown(cocos2d::EventMouse* pEvent);
	void onMouseButtonUp(cocos2d::EventMouse* pEvent);
	
	void setActionButtonState(bool bIsActive, const MouseButtonCode& keyCode);
	void setStateButtonState(bool bIsPressed, const MouseButtonCode& keyCode);
};
