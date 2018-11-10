#include "MainMenuScene.h"
#include "LoadingScreenScene.h"
#include "Input/GameInput.h"
#include "ui/CocosGUI.h"
#include "Utils/Utils.h"
#include "GameConsts.h"
#include "ENextScene.h"

USING_NS_CC;

MainMenuScene::MainMenuScene()	
{

}

Scene* MainMenuScene::CreateScene()
{
	return create();
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

	auto layout = ui::Layout::create();	
	layout->setNormalizedPosition(Vec2(0.43f, 0.6f));
	layout->setLayoutType(ui::Layout::Type::VERTICAL);
	layout->setSizeType(ui::Widget::SizeType::PERCENT);
	auto relativeParam = ui::RelativeLayoutParameter::create();
	relativeParam->setAlign(ui::RelativeAlign::CENTER_IN_PARENT);	
	
	layout->setLayoutParameter(relativeParam);

	layout->addChild(startButton);
	layout->addChild(exitButton);

	auto title = ui::Text::create("Best title ever !", "Arial", 80);
	title->setTextHorizontalAlignment(TextHAlignment::LEFT);
	title->setNormalizedPosition(Vec2(0.52f, 0.8f));

	addChild(title);
	addChild(layout);	
    
    return true;
}

void MainMenuScene::OnStartPressed()
{	
	Utils::StartTimerWithCallback(this,
		CC_CALLBACK_0(MainMenuScene::SwitchToGameScene, this),
		GameConsts::LOADING_TIME);
}

void MainMenuScene::OnQuitPressed()
{
	Director::getInstance()->end();
}

void MainMenuScene::SwitchToGameScene()
{	
	Director::getInstance()->replaceScene(LoadingScreenScene::CreateScene(ENextScene::GAME));	
}
