#pragma once

#include "LightSoulsTypes.h"
#include "tinyxml2/tinyxml2.h"

NS_LIGHTSOULS_BEGIN

class GameInput;
class World;
class ProgressBar;
class AIAgentManager;
class Entity;
class UIElementConfig;
enum class GameInputType;

using XMLNode = tinyxml2::XMLNode;
using XMLElement = tinyxml2::XMLElement;
using XMLDoc = tinyxml2::XMLDocument;
using XMLError = tinyxml2::XMLError;
using LoadInputCallback = std::function<void(GameInput&, const XMLElement*,
	const String&)>;

class XMLLoader
{
public:
	static bool InitializeEntityUsingXMLFile(Entity& entity,
		const String& pathToXML);
	static bool LoadInputSettings(GameInput& gameInput, 
		const String& pathToConfigXml);
	static bool InitializeUIProgressBar(ProgressBar& healthBar, const String& pathToXML);
	static bool LoadWorld(World& world, const String& pathToXML);
	static bool InitializeAIManagerUsingXMLFile(AIAgentManager& aiManager,
		const String& pathToXML);

private:
	XMLLoader();
		
	static bool						LoadXMLFile(const String& pathToXML, XMLDoc& outDoc);

	static void						LoadActionTriggers(GameInput& gameInput,
		const XMLElement* pElement, LoadInputCallback onKeyboardInput,
		LoadInputCallback onMouseInput, LoadInputCallback onGameControllerInput);

	static void						LoadKeyboardAxis(GameInput& gameInput,
		const XMLElement* pElement, const String& actionName);
	static void						LoadGameControllerAxis(GameInput& gameInput,
		const XMLElement* element, const String& actionName);
	
	static void						LoadActionButton(GameInput& gameInput, GameInputType inputType,
		const XMLElement* element, const String& actionName, const String& xmlAttributeName);

	static void						LoadUIElement(const XMLElement* element, UIElementConfig& outUIElement);
	static void						LoadPhysicsMaterialFromAttributes(const XMLNode* pNode, cocos2d::PhysicsMaterial& outMaterial);
	static void						LoadVector3FromAttributes(const XMLElement* node, cocos2d::Vec3& outResult);
	static void						LoadVector2FromAttributes(const XMLElement* element, cocos2d::Vec2& outResult);
	static void						CreatePhysicsBodyFromAttributes(Entity& attachmentEntity,
										const XMLNode* xmlNode,
										cocos2d::Size& outSize);

	static GameInputType			StrToGameInputType(const String& inputType);
};

NS_LIGHTSOULS_END
