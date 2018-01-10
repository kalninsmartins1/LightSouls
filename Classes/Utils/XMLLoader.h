#pragma once

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

class GameInput;
class World;
enum class GameInputType;

namespace tinyxml2
{
	class XMLNode;
}

typedef tinyxml2::XMLNode XMLNode;
typedef tinyxml2::XMLElement XMLElement;
typedef tinyxml2::XMLDocument XMLDoc;
typedef tinyxml2::XMLError XMLError;
typedef std::function<void(GameInput&, const XMLElement*, const char*)> LoadInputCallback;


class XMLLoader
{
public:
	static bool initializeSpriteUsingXMLFile(cocos2d::Sprite& sprite,
		const char* pathToXML);
	static bool loadInputSettings(GameInput& gameInput, const char* pathToConfigXml);	
	static bool loadWorld(World& world, const char* pathToXML);

private:
	XMLLoader();	

	static void loadActionTriggers(GameInput& gameInput,
		const XMLElement* pElement, LoadInputCallback onKeyboardInput,
		LoadInputCallback onMouseInput, LoadInputCallback onGameControllerInput);

	static void loadKeyboardAxis(GameInput& gameInput,
		const XMLElement* pElement, const char* actionName);
	static void loadGameControllerAxis(GameInput& gameInput,
		const XMLElement* pElement, const char* actionName);
	
	static void loadActionButton(GameInput& gameInput, GameInputType inputType,
		const XMLElement* pElement, const char* actionName, const char* xmlAttributeName);

	static cocos2d::PhysicsMaterial loadPhysicsMaterialFromAttributes(
		const XMLNode* pNode);
	static cocos2d::Vec3 XMLLoader::loadVec3FromAttributes(
		const XMLNode* pNode);
	static cocos2d::PhysicsBody* XMLLoader::loadPhysicsBodyFromAttributes(
		const XMLNode* pNode);

	static GameInputType strToGameInputType(const char* inputType);
};
