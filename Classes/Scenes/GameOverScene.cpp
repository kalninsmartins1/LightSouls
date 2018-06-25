#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "ScoringSystem/ScoringSystem.h"

USING_NS_CC;

Scene* GameOverScene::CreateScene()
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
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto tryAgainButton = ui::Button::create("res/Graphics/UI/MainMenu/button.png");
	auto goToMenuButton = ui::Button::create("res/Graphics/UI/MainMenu/button.png");
	tryAgainButton->setTitleText("Try again");
	tryAgainButton->setTitleFontSize(18.0f);
	goToMenuButton->setTitleText("Go to menu");
	goToMenuButton->setTitleFontSize(18.0f);

	tryAgainButton->addClickEventListener(CC_CALLBACK_0(GameOverScene::OnTryAgainPressed, this));
	goToMenuButton->addClickEventListener(CC_CALLBACK_0(GameOverScene::OnGoToMenuPressed, this));

	auto title = ui::Text::create("Game over !", "Arial", 80);
	auto scoreText = ui::Text::create(StringUtils::format("Score: %d", LightSouls::ScoringSystem::GetInstance()->GetScore()), "Arial", 40);

	auto layout = ui::Layout::create();
	layout->setNormalizedPosition(Vec2(0.43f, 0.70f));
	layout->setLayoutType(ui::Layout::Type::VERTICAL);
	auto linearLayoutParam = ui::LinearLayoutParameter::create();
	linearLayoutParam->setGravity(ui::LinearGravity::CENTER_VERTICAL);
	layout->setLayoutParameter(linearLayoutParam);

	layout->addChild(title);
	layout->addChild(scoreText);
	layout->addChild(tryAgainButton);
	layout->addChild(goToMenuButton);

	addChild(layout);


    return true;
}

void GameOverScene::OnTryAgainPressed()
{
	Director::getInstance()->replaceScene(GameScene::CreateScene());
}

void GameOverScene::OnGoToMenuPressed()
{
	Director::getInstance()->replaceScene(MainMenuScene::CreateScene());
}
