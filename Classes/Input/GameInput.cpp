#include "GameInput.h"
#include "tinyxml2/tinyxml2.h"
#include "Particle3D/PU/CCPUOnEventFlagObserver.h"
#include "Utils/XMLLoader.h"

using namespace cocos2d;

GameInput::GameInput()
{
	XMLLoader::loadInputSettings(*this, "res/Configs/Input/Input.xml");
}

GameInput* GameInput::getInstance()
{
	static GameInput instance;
	return &instance;
}

bool GameInput::init(const char* configFilePath)
{
	// Load the file
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(configFilePath);

	bool isInitSuccessful = true;
	if (err)
	{
		log("XMLLoader: [initializeSpriteUsingXMLFile] XML file not found: %s", configFilePath);
		isInitSuccessful = false;
	}
	else
	{
	}
	
	return isInitSuccessful;
}

Vec2 GameInput::getInputAxis(const char* axis) const
{
	return Vec2::ZERO;
}

bool GameInput::HasActionInput(const char* action) const
{
	return true;
}

void GameInput::addAxisInput(const char* actionName, const char* keyCode, const char* axisType, float minValue,
	float maxValue)
{

}

void GameInput::addActionInput(const char* actionName, const char* buttonCode)
{

}

void GameInput::addStateInput(const char* actionName, const char* buttonName)
{

}
