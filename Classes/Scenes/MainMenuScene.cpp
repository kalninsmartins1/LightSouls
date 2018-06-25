#include "MainMenuScene.h"
#include "GameScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* MainMenuScene::CreateScene()
{
	// create the scene with physics enabled
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
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto startButton = ui::Button::create("res/Graphics/UI/MainMenu/button.png");
	auto exitButton = ui::Button::create("res/Graphics/UI/MainMenu/button.png");
	startButton->setTitleText("Start");
	startButton->setTitleFontSize(18.0f);
	exitButton->setTitleText("Quit");
	exitButton->setTitleFontSize(18.0f);

	startButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::OnStartPressed, this));
	exitButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::OnQuitPressed, this));	

	auto title = ui::Text::create("Best title ever !", "Arial", 80);
	title->setTextHorizontalAlignment(TextHAlignment::CENTER);

	auto layout = ui::Layout::create();	
	layout->setNormalizedPosition(Vec2(0.43f, 0.5f));
	layout->setLayoutType(ui::Layout::Type::VERTICAL);
	auto linearLayoutParam = ui::LinearLayoutParameter::create();
	linearLayoutParam->setGravity(ui::LinearGravity::CENTER_VERTICAL);
	layout->setLayoutParameter(linearLayoutParam);

	layout->addChild(title);
	layout->addChild(startButton);
	layout->addChild(exitButton);

	addChild(layout);
	
    
    return true;
}

void MainMenuScene::OnStartPressed()
{
	Director::getInstance()->replaceScene(GameScene::CreateScene());
}

void MainMenuScene::OnQuitPressed()
{
	Director::getInstance()->end();
}
