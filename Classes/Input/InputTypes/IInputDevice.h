#pragma once

class IInputDevice
{
public:
	// Returns true for action once when input is released
	virtual bool hasAction(const std::string& action) const = 0;

	// Returns true while input for action is not released
	virtual bool hasActionState(const std::string& action) const = 0;
};