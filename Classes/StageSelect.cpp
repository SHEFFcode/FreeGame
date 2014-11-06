#include "StageSelect.h"
#include "Definitions.h"
#include "MainMenu.h"
#include "Stage.h"

USING_NS_CC;

unsigned int set; // which set of stages is shown 0 or 1

Scene* StageSelect::createScene( unsigned int setRef )
{
    set = setRef;
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StageSelect::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StageSelect::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vector<MenuItem*> menuArray(12);
    std::vector<std::string> stageFiles = { "red.jpg", "red.jpg", "red.jpg", "red.jpg", "red.jpg", "red.jpg", "red.jpg", "red.jpg", "red.jpg", "red.jpg" };
    
    auto width = (visibleSize.width * 0.8 - 100) / 5;
    auto height = visibleSize.height * 0.7;
    auto xStart = visibleSize.width * 0.1;
    auto yStart = visibleSize.height * 0.2;
    
    int mStart, mEnd;
    
    if(!set){
        mStart = 0;
        mEnd = 5;
    } else {
        mStart = 5;
        mEnd = 10;
    }
    
    for (int i=0; i<5; i++){
        if(i != 0 ){
            xStart = xStart + width + 25;
        }
        __String *stageNum = __String::createWithFormat("%i", i + 1 + mStart);
        
        auto stage = Sprite::create(stageFiles.at(i), Rect(xStart, yStart, width, height));
        auto item = MenuItemSprite::create(stage, stage, stage, CC_CALLBACK_1(StageSelect::Stage, this));
        item->setPosition(Point(xStart + ( width / 2), yStart + ( height / 2)));
        item->setTag(i + 1 + mStart);
        menuArray.insert(i, item);
        
        auto num = Label::createWithTTF(stageNum->getCString(), "Action Man.ttf", visibleSize.height * 0.1 );
        num->setColor(Color3B::WHITE);
        num->setPosition(Point( xStart + num->getContentSize().width / 2 + 10, yStart + height - num->getContentSize().height / 2 - 10));
        this->addChild(num,1);
    }
    
    auto main_menu = MenuItemImage::create("main_menu.png", "main_menu.png", "main_menu.png", CC_CALLBACK_1(StageSelect::GoToMainMenu, this));
    main_menu->setPosition(Point( visibleSize.width * 0.05 + ( main_menu->getContentSize().width / 2), visibleSize.height * 0.1));
    menuArray.insert(5, main_menu);
    
    if(!set) {
        auto arrow = MenuItemImage::create("arrow_right.jpg", "arrow_right.jpg", "arrow_right.jpg", CC_CALLBACK_1(StageSelect::GoToStageSelect, this));
        arrow->setPosition(Point(visibleSize.width - 10 - arrow->getContentSize().width / 2, visibleSize.height / 2));
        menuArray.insert(6, arrow);
    } else {
        auto arrow = MenuItemImage::create("arrow_left.jpg", "arrow_left.jpg", "arrow_left.jpg", CC_CALLBACK_1(StageSelect::GoToStageSelect, this));
        arrow->setPosition(Point(10 + arrow->getContentSize().width / 2, visibleSize.height / 2));
        menuArray.insert(6, arrow);
    }

    // Menu
    auto menu = Menu::createWithArray(menuArray);
    menu->setPosition(Point::ZERO);
    
    this->addChild(menu);

    
    return true;
}

void StageSelect::Stage( cocos2d::Ref *sender )
{
    MenuItem* pMenuItem = (MenuItem *)(sender);
    
    int tag = (int)pMenuItem->getTag();
    
    auto scene = Stage::createScene(tag, 0); // stage num, starting levelset default
    
    Director::getInstance()->replaceScene(scene);

}

void StageSelect::GoToStageSelect( cocos2d::Ref *sender )
{
    if(!set)
        set++;
    else
        set=0;

    auto scene = StageSelect::createScene(set);
    
    Director::getInstance()->replaceScene(scene);
}

void StageSelect::GoToMainMenu(cocos2d::Ref *sender)
{
    auto scene = MainMenu::createScene();
    
    Director::getInstance()->replaceScene(scene);
}











