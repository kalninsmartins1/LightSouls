#include "GameScene.h"
#include "World/Entity/Player/Player.h"
#include "Classes/Core/Input/GameInput.h"
#include "World/World.h"
#include "World/Entity/CustomActions/CameraFallow.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "Classes/Core/Events/ValueChangedEventData.h"
#include "Classes/Core/Camera/Camera.h"
#include "LoadingScreenScene.h"
#include "ENextScene.h"
#include "ScoringSystem/ScoringSystem.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"
#include "Classes/Core/UI/Configs/Loaders/ElementConfigLoader.h"
#include "Classes/Core/UI/Configs/Loaders/ImageAttributeLoader.h"
#include "Classes/Core/UI/Configs/Loaders/TransformAttributeLoader.h"
#include "Classes/Core/UI/Configs/Loaders/StackPanelAttributeLoader.h"
#include "Classes/Core/UI/Configs/Loaders/VerticalPanelAttributeLoader.h"
#include "Classes/Core/UI/Configs/Loaders/ProgressBarAttributeLoader.h"
#include "Classes/Core/UI/Element.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PhysicsManager* GameScene::s_physicsManager = nullptr;
GameInput* GameScene::s_gameInput = nullptr;

GameScene::GameScene()
	: m_gameSpeedModifier(*this, *getEventDispatcher())
	, m_player(nullptr)
	, m_scoreText(nullptr)
	, m_physicsDebugEnabled(false)
	, m_timeModifier(1.0f)
{
	// Reset player score upon new game
	ScoringSystem* scoringSystem = ScoringSystem::GetInstance();
	if (scoringSystem != nullptr)
	{
		scoringSystem->Reset();
	}
}

GameScene::~GameScene()
{
	AIAgentManager* aiManager = AIAgentManager::GetInstance();
	if (aiManager != nullptr)
	{
		aiManager->Cleanup();
	}
	
	s_physicsManager->release();
	s_physicsManager = nullptr;
	s_gameInput->release();
	s_gameInput = nullptr;
}

PhysicsManager* GameScene::GetPhysicsManager()
{
	return s_physicsManager;
}

GameInput* GameScene::GetGameInput()
{
	return s_gameInput;
}

float GameScene::GetTimeModifier() const
{
	return m_timeModifier;
}

void GameScene::SetTimeModifier(float modifier)
{
	m_timeModifier = modifier;
	if (m_player != nullptr)
	{
		m_player->SetTimeModifier(modifier);
	}

	AIAgentManager* aiManager = AIAgentManager::GetInstance();
	if (aiManager != nullptr)
	{
		aiManager->SetTimeModifier(modifier);
	}
}

Scene* GameScene::CreateScene()
{
	// Create the scene with physics enabled
	auto scene = createWithPhysics();

	Node* layer = create();
	scene->addChild(layer);

	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}	

	RegisterSystems();

	// Init physics manager
	s_physicsManager = PhysicsManager::Create(this);
	if (s_physicsManager == nullptr)
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize PhysicsManager !");
	}

	// Init Input
	s_gameInput = GameInput::Create("res/Configs/Input/Input.xml");
	if (s_gameInput == nullptr)
	{
		// Halt the game when in debug mode
		CCASSERT(false, "GameScene: Failed to load input configuration !");
	}

	Node* worldLayer = InitWolrdLayer();
	InitUILayer();	

	m_gameSpeedModifier.Init("res/Configs/World/GameSpeedModifier.xml");	
	InitVFXManger(worldLayer);
	RegisterForEvents();
	scheduleUpdate();
	Utils::ToggleMouseCursor(false);

    return true;
}

void GameScene::update(float deltaTime)
{
	deltaTime += m_timeModifier;

	// Keep input events up to date
	s_gameInput->Update(deltaTime);
	if (s_gameInput->HasAction("ExitGame"))
	{
		Director::getInstance()->end();
		return;
	}

	// Update player
	if (m_player != nullptr)
	{
		m_player->Update(deltaTime);
	}

	// Update AI
	AIAgentManager::GetInstance()->Update(deltaTime);	

#if !defined(LIGHTSOULS_RELEASE)
	if (s_gameInput->HasAction("ReloadGame"))
	{
		ReloadGame();
	}
	
	ProcessDebugPhysicsDraw();
#endif
}

void GameScene::RegisterSystems()
{
	RegisterAttributeLoaders();
}

void GameScene::RegisterAttributeLoaders()
{
	UI::ElementConfigLoader::RemoveAllConfigLoaders();
	UI::ElementConfigLoader::RegisterConfigLoader(UI::ImageAttributeLoader());
	UI::ElementConfigLoader::RegisterConfigLoader(UI::TransformAttributeLoader());
	UI::ElementConfigLoader::RegisterConfigLoader(UI::StackPanelAttributeLoader());
	UI::ElementConfigLoader::RegisterConfigLoader(UI::VerticalPanelAttributeLoader());
	UI::ElementConfigLoader::RegisterConfigLoader(UI::ProgressBarAttributeLoader());
}

Node* GameScene::InitWolrdLayer()
{
	Node* worldLayer = Node::create();

	// Init world
	World* world = World::Create("res/Configs/World/WorldConfig.xml");
	worldLayer->addChild(world);

	// Init player
	m_player = Player::Create("res/Configs/World/Player/Player.xml");
	worldLayer->addChild(m_player);

	// Init AI
	AIAgentManager* agentManager = AIAgentManager::GetInstance();
	if (agentManager != nullptr && agentManager->Init(worldLayer, "res/Configs/World/AI/AIManager.xml"))
	{
		agentManager->SetTargetEntity(m_player);
	}
	else
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize AI manager !");
	}

	// Set world camera mask
	worldLayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER1));
	worldLayer->setCascadeOpacityEnabled(true);
	addChild(worldLayer);

	// Create world camera and set it to follow player
	LS::Camera* worldCamera = LS::Camera::Create("res/Configs/Camera/Camera.xml");
	worldCamera->setCameraFlag(CameraFlag::USER1);
	worldCamera->setPosition(m_player->getPosition()); // Make the player position in middle of camera	
	addChild(worldCamera);

	return worldLayer;	
}

void GameScene::InitUILayer()
{
	// Init UI
	Node* uiLayer = Node::create();
	uiLayer->setContentSize(Utils::GetScreenSize());
	
	Sprite* screenOverlay = Sprite::create("res/Graphics/UI/screenOverlay.png");
	const Vector2& scale = Utils::GetScreenFillScale(screenOverlay->getContentSize());
	screenOverlay->setScale(scale.GetX(), scale.GetY());
	screenOverlay->setAnchorPoint(Vec2::ZERO);

	UI::ElementConfig config;
	UI::ElementConfigLoader::LoadConfig("res/Configs/UI/InGameIndicators/IndicatorPanelStack.xml", config);
	UI::Element* indicatorPanel = config.Create();
	if (indicatorPanel == nullptr)
	{
		CCLOG("GameScene: Failed to initialize game indicator panel !");
	}

	m_scoreText = ui::Text::create(StringUtils::format("Score: %d", ScoringSystem::GetInstance()->GetScore()),
		GameConsts::DEFAULT_FONT, 100);
	m_scoreText->setNormalizedPosition(Vec2(0.5f, 0.82f)); 

	uiLayer->addChild(screenOverlay);
	uiLayer->addChild(indicatorPanel);
	uiLayer->addChild(m_scoreText);	
	uiLayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER2));	
	addChild(uiLayer);

	// Create UI camera
	cocos2d::Camera* uiCamera = cocos2d::Camera::create();
	uiCamera->setCameraFlag(CameraFlag::USER2);
	addChild(uiCamera);
}

void GameScene::StartGameOverFadeIn(float time)
{
	Sprite* fadeSprite = Sprite::create("res/Graphics/pixel.png");
	const Vector2& scale = Utils::GetScreenFillScale(fadeSprite->getContentSize());
	fadeSprite->setScale(scale.GetX(), scale.GetY());
	fadeSprite->setColor(Color3B::BLACK);
	fadeSprite->setAnchorPoint(Vec2::ZERO);
	fadeSprite->setOpacity(0);
	fadeSprite->runAction(FadeIn::create(time));

	auto infoText = ui::Text::create("Died !", GameConsts::DEFAULT_FONT, 100);
	infoText->setTextColor(Color4B::RED);
	infoText->setNormalizedPosition(Vec2(0.5f, 0.65f));

	Node* fadeContainer = Node::create();
	fadeContainer->setContentSize(Utils::GetScreenSize());
	fadeContainer->addChild(fadeSprite);
	fadeContainer->addChild(infoText);

	// Needs to be called once all the children has been added
	fadeContainer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER2));
	addChild(fadeContainer);
}

void GameScene::OnPlayerDisappeared()
{
	float toNextSceneTime = 2.0f;
	StartGameOverFadeIn(toNextSceneTime);
	Utils::StartTimerWithCallback(this,
		CC_CALLBACK_0(GameScene::SwitchToGameOverScene, this),
		toNextSceneTime);
}

void GameScene::OnAgentDestroyed(cc::EventCustom* eventData)
{	
	ScoringSystem* scoringSystem = ScoringSystem::GetInstance();
	scoringSystem->IncreaseScore();
	m_scoreText->setString(StringUtils::format("Score: %d", scoringSystem->GetScore()));
}

void GameScene::ReloadGame()
{
	unscheduleUpdate();
	m_player = nullptr;
	removeAllChildren();
	auto aiAgentManager = AIAgentManager::GetInstance();
	if (aiAgentManager != nullptr)
	{
		aiAgentManager->Cleanup();
	}

	auto physicsManager = GetPhysicsManager();
	if (physicsManager != nullptr)
	{
		physicsManager->OnReload();
	}

	Node* worldLayer = InitWolrdLayer();
	InitVFXManger(worldLayer);
	InitUILayer();
	scheduleUpdate();
}

void GameScene::RegisterForEvents()
{
	EventDispatcher* eventDispatcher = getEventDispatcher();
	if (eventDispatcher != nullptr)
	{
		eventDispatcher->addCustomEventListener(Player::GetEventOnPlayerDisappeared(),
			CC_CALLBACK_0(GameScene::OnPlayerDisappeared, this));
		eventDispatcher->addCustomEventListener(AIAgent::GetEventOnDisappeared(),
			CC_CALLBACK_1(GameScene::OnAgentDestroyed, this));
	}
}

void GameScene::InitVFXManger(Node* worldLayer)
{
	m_vfxManager.Init(worldLayer, "res/Configs/World/VFX/VFXManager.xml");
}

void GameScene::SwitchToGameOverScene()
{
	EventDispatcher* dispatcher = getEventDispatcher();
	if (dispatcher != nullptr)
	{
		dispatcher->removeAllEventListeners();
	}

	Director* director = Director::getInstance();
	if (director != nullptr)
	{
		director->replaceScene(LoadingScreenScene::CreateScene(ENextScene::GAME_OVER));
	}
}

void GameScene::ProcessDebugPhysicsDraw()
{
	if (s_gameInput->HasAction("DebugPhysics"))
	{
		m_physicsDebugEnabled = !m_physicsDebugEnabled;
	}
	
	PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	if (world != nullptr)
	{
		if (m_physicsDebugEnabled)
		{
			world->setDebugDrawMask(0xFFFFFF);
			world->setDebugDrawCameraMask(CameraFlag::USER1);
		}
		else if(world->getDebugDrawMask() != 0)
		{
			world->setDebugDrawMask(0);
			world->setDebugDrawCameraMask(CameraFlag::USER8);
		}
	}	
}
