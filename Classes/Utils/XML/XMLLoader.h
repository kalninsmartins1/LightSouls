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
class Camera;
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
	static bool InitializeAIManagerUsingXMLFile(AIAgentManager& aiManager,
		const String& pathToXML);

	static bool InitializeUIProgressBar(ProgressBar& healthBar, const String& pathToXML);
	static bool LoadWorld(World& world, const String& pathToXML);		
	static bool InitializeCamera(Camera& camera, const String& pathToXML);
	static void	ReadXMLAttribute(const XMLElement* element, const String& attributeName, String& outValue);	

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
	static void						GetVector3FromElement(const XMLElement* node, cocos2d::Vec3& outResult);
	static void						GetVector2FromElement(const XMLElement* element, cocos2d::Vec2& outResult);
	static void						CreatePhysicsBodyFromAttributes(cocos2d::Node* attachmentNode,
										const XMLNode* xmlNode,
										cocos2d::Size& outSize);
	static void						LoadNodeComponents(cocos2d::Node* node, const XMLElement* root);
	static GameInputType			StrToGameInputType(const String& inputType);	
};

NS_LIGHTSOULS_END
