#pragma once
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

class GameInput;
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

private:
	XMLLoader();

	static void loadActionTriggers(GameInput& gameInput,
		const XMLElement* pElement, LoadInputCallback onKeyboardInput,
		LoadInputCallback onMouseInput);

	static void loadAxisKeyButtons(GameInput& gameInput,
		const XMLElement* pElement, const char* actionName);
	static void loadMouseButton(GameInput& gameInput,
		const XMLElement* pElement, const char* actionName);
	static void loadActionKeyButton(GameInput& gameInput,
		const XMLElement* pElement, const char* actionName);

	static cocos2d::PhysicsMaterial loadPhysicsMaterialFromAttributes(
		const XMLNode* pNode);
	static cocos2d::Vec3 XMLLoader::loadVec3FromAttributes(
		const XMLNode* pNode);
	static cocos2d::PhysicsBody* XMLLoader::loadPhysicsBodyFromAttributes(
		const XMLNode* pNode);

	static GameInputType strToGameInputType(const char* inputType);
};
