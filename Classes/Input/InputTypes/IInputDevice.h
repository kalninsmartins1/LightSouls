#pragma once

class IInputDevice
{
public:

	// Returns true for action once when input is released
	virtual bool hasAction(const std::string& action) const = 0;

	// Returns true while input for action is not released
	virtual bool hasActionState(const std::string& action) const = 0;

	// Returs current axis input value normalized in range 0 to 1
	virtual float getAxisInput(const std::string& axisName) const = 0;

	// Returns true if this input device has specified axis input
	virtual bool hasAxisInput(const std::string& axisName) const = 0;
};