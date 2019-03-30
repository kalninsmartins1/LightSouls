#pragma once

#include "LightSoulsTypes.h"

namespace LS
{
	class Camera;
};

class GameInput;
class World;
class ProgressBar;
class AIAgentManager;
class Entity;
class UIElementConfig;
class Camera;
class ProjectileConfig;
class PhysicsBodyConfig;
class GameSpeedModifier;
enum class GameInputType;

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
	static bool InitializeProjectileConfig(ProjectileConfig& config, const String& pathToXML);
	static bool InitializeComponents(cc::Node& node, const String& pathToXML);
	static bool InitializeComponent(cc::Node& node, const XMLElement& element, const String& componentType);
	static bool InitializeGameSpeedModifier(GameSpeedModifier& speedModifier, const String& pathToXML);

	static bool InitializeUIProgressBar(ProgressBar& healthBar, const String& pathToXML);
	static bool LoadWorld(World& world, const String& pathToXML);
	static bool InitializeCamera(LS::Camera& camera, const String& pathToXML);
	static void	ReadXMLAttribute(const XMLElement* element, const String& attributeName, String& outValue);

private:
	XMLLoader();

private:
	static bool						GetAnchorPosition(const XMLElement& element, Vector2& outPos);
	static bool						LoadXMLFile(const String& pathToXML, XMLDoc& outDoc);

	static void						LoadActionTriggers(GameInput& gameInput,
		const XMLElement* pElement, LoadInputCallback onKeyboardInput,
		LoadInputCallback onMouseInput, LoadInputCallback onGameControllerInput);

	static void						LoadBackgroundActions(cc::Node& noed, const XMLElement& xmlElement);
	static void						LoadKeyboardAxis(GameInput& gameInput,
		const XMLElement* pElement, const String& actionName);
	static void						LoadGameControllerAxis(GameInput& gameInput,
		const XMLElement* element, const String& actionName);

	static void						LoadActionButton(GameInput& gameInput, GameInputType inputType,
		const XMLElement* element, const String& actionName, const String& xmlAttributeName);

	static void						LoadUIElement(const XMLElement& element, UIElementConfig& outUIElement);
	static void						LoadPhysicsMaterialFromAttributes(const XMLElement& element, cocos2d::PhysicsMaterial& outMaterial);
	static void						GetVector3FromElement(const XMLElement& element, Vector3& outResult);
	static void						GetVector2FromElement(const XMLElement& element, Vector2& outResult);
	static void						CreatePhysicsBodyFromAttributes(const XMLElement& xmlElement, PhysicsBodyConfig& outConfig);
	static void						LoadNodeComponents(cc::Node& node, const XMLElement& root);
	static GameInputType			StrToGameInputType(const String& inputType);
};


