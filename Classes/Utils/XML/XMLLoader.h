#pragma once

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
#include "World/Entity/AI/AIAgentManager.h"

class GameInput;
class World;
enum class GameInputType;

using XMLNode = tinyxml2::XMLNode;
using XMLElement = tinyxml2::XMLElement;
using XMLDoc = tinyxml2::XMLDocument;
using XMLError = tinyxml2::XMLError;
using LoadInputCallback = std::function<void(GameInput&, const XMLElement*,
	const std::string&)>;

class XMLLoader
{
public:
	static bool InitializeEntityUsingXMLFile(Entity& entity,
		const std::string& pathToXML);
	static bool LoadInputSettings(GameInput& gameInput, 
		const std::string& pathToConfigXml);
	static bool LoadWorld(World& world, const std::string&);
	static bool InitializeAIManagerUsingXMLFile(AIAgentManager& aiManager,
		const std::string& pathToXML);

private:
	XMLLoader();
		
	static void LoadActionTriggers(GameInput& gameInput,
		const XMLElement* pElement, LoadInputCallback onKeyboardInput,
		LoadInputCallback onMouseInput, LoadInputCallback onGameControllerInput);

	static void LoadKeyboardAxis(GameInput& gameInput,
		const XMLElement* pElement, const std::string& actionName);
	static void LoadGameControllerAxis(GameInput& gameInput,
		const XMLElement* pElement, const std::string& actionName);
	
	static void LoadActionButton(GameInput& gameInput, GameInputType inputType,
		const XMLElement* pElement, const std::string& actionName, const std::string& xmlAttributeName);

	static cocos2d::PhysicsMaterial LoadPhysicsMaterialFromAttributes(
		const XMLNode* pNode);
	static cocos2d::Vec3 XMLLoader::LoadVec3FromAttributes(
		const XMLNode* pNode);
	static void XMLLoader::CreatePhysicsBodyFromAttributes(
		cocos2d::Node& attachmentNode,
		const XMLNode* xmlNode,
		cocos2d::Size& outSize);

	static GameInputType StrToGameInputType(const std::string& inputType);
};
