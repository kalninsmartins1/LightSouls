#pragma once

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
#include "World/Entity/AI/AIAgentManager.h"

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
typedef std::function<void(GameInput&, const XMLElement*,
	const std::string&)> LoadInputCallback;


class XMLLoader
{
public:
	static bool initializeSpriteUsingXMLFile(cocos2d::Sprite& sprite,
		const std::string& pathToXML);
	static bool loadInputSettings(GameInput& gameInput, 
		const std::string& pathToConfigXml);
	static bool loadWorld(World& world, const std::string&);
	static bool initializeAIManagerUsingXMLFile(AIAgentManager& aiManager,
		const std::string& pathToXML);

private:
	XMLLoader();
		
	static void loadActionTriggers(GameInput& gameInput,
		const XMLElement* pElement, LoadInputCallback onKeyboardInput,
		LoadInputCallback onMouseInput, LoadInputCallback onGameControllerInput);

	static void loadKeyboardAxis(GameInput& gameInput,
		const XMLElement* pElement, const std::string& actionName);
	static void loadGameControllerAxis(GameInput& gameInput,
		const XMLElement* pElement, const std::string& actionName);
	
	static void loadActionButton(GameInput& gameInput, GameInputType inputType,
		const XMLElement* pElement, const std::string& actionName, const std::string& xmlAttributeName);

	static cocos2d::PhysicsMaterial loadPhysicsMaterialFromAttributes(
		const XMLNode* pNode);
	static cocos2d::Vec3 XMLLoader::loadVec3FromAttributes(
		const XMLNode* pNode);
	static cocos2d::PhysicsBody* XMLLoader::loadPhysicsBodyFromAttributes(const XMLNode* pNode);

	static GameInputType strToGameInputType(const std::string& inputType);
};
