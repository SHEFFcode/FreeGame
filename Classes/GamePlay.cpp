#include "GamePlay.h"
#include "Obstacle.h"
#include "Definitions.h"
#include "Player.h"
#include "MainMenu.h"

USING_NS_CC;

unsigned int stageLoaded, levelLoaded;

Scene* GamePlay::createScene(unsigned int stageRef, unsigned int levelRef)
{
    stageLoaded = stageRef;
    levelLoaded = levelRef;
    
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0,0));
    
    // 'layer' is an autorelease object
    auto layer = GamePlay::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePlay::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    visibleWidth = visibleSize.width;
    visibleHeight = visibleSize.height;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->Setup();  // Load the stage / level
    
    
    // setup bounding box
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask( BORDER_COLLISION_BITMASK );
    edgeBody->setContactTestBitmask( true );
    
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);
    
    this->addChild(edgeNode);
    
    // setup physics
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( GamePlay::onContactBegin, this );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    //setup touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( GamePlay::onTouchBegan, this );
    touchListener->onTouchMoved = CC_CALLBACK_2( GamePlay::onTouchMoved, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, player->puff
                                                             );
    
    scheduleUpdate();
    
    return true;
}

bool GamePlay::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        //CCLOG("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
        if(levelStarted == 0) {
            mapAction = RepeatForever::create(MoveBy::create(MOVEMENT_SPEED * visibleWidth, Point(-visibleWidth * 1.5, 0)));
            map->runAction(mapAction);
        }
        levelStarted = 1;
        
        return true;
    }
    return false;
}


void GamePlay::onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *event )
{
    Vec2 touchPos = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

    player->UpdatePosition(touchPos);
}

bool GamePlay::onContactBegin( cocos2d::PhysicsContact &contact )
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    // Obstacle Collision
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        // stops all movement
        GamePlay::stopAction(mapAction);
        
        // remove all touch events
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->getEventDispatcher()->release();
        
        a->setEnable(false);
        a->setContactTestBitmask(false);
        b->setEnable(false);
        b->setContactTestBitmask(false);
        
        // add game over popover
        auto popover = Node::create();
        popover->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
        
        auto background = Sprite::create("red.jpg", Rect(100, 100, visibleSize.width - 200, visibleSize.height - 200));
        background->setOpacity(0);
        auto reveal = FadeIn::create(1);
        background->runAction(reveal);
        
        popover->addChild(background);
        
        auto replay = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(GamePlay::GoToReplayLevel, this));
        replay->setPosition(Point(visibleSize.width / 2 - 100, visibleSize.height / 2));

        auto mainMenu = MenuItemImage::create("options.png", "options.png", CC_CALLBACK_1(GamePlay::GoToMainMenu, this));
        mainMenu->setPosition(Point(visibleSize.width / 2 - 100, (visibleSize.height / 2) - 210));
        
        auto menu = Menu::create(replay, mainMenu, NULL);
        menu->setPosition(Point::ZERO);
        menu->setOpacity(0);
        auto reveal2 = FadeIn::create(1);
        menu->runAction(reveal2);
        
        background->addChild(menu);

        
        this->addChild(popover, 10000);
    }
    
    // Collection Collision
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && COLLECTION_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && COLLECTION_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        b->setEnable(false);
        b->setContactTestBitmask(false);
        
        // Remove tile
        float mapHeight = map->getMapSize().height * 32;
        int x = round(b->getNode()->getPosition().x / 32);
        int y = round(( mapHeight - b->getNode()->getPosition().y) / 32 - 1);
        Point point = Point(x,y);
        map->getLayer("Collection")->removeTileAt(point);
        
        // Change score
        score++;
        __String *tempScore = __String::createWithFormat("%i", score);
        scoreLabel->setString(tempScore->getCString());
    }
    
    // FinishLine Collision
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && FINISH_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && FINISH_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        // remove all touch events
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->getEventDispatcher()->release();
        
        a->setEnable(false);
        a->setContactTestBitmask(false);
        b->setEnable(false);
        b->setContactTestBitmask(false);
        
        auto finishingMove = MoveBy::create(MOVEMENT_SPEED * (visibleWidth / 2), Point(visibleWidth / 2 - player->puff->getPositionX(), visibleHeight / 2 - player->puff->getPositionY()));
        
        player->puff->runAction(finishingMove);
    }
    
    return true;
}

void GamePlay::Setup()
{
    __String *level = __String::createWithFormat("tiles/%i_%i.tmx", stageLoaded,levelLoaded);
    map = TMXTiledMap::create(level->getCString());
    map->setAnchorPoint(Vec2(0, 0));

    float map_height = map->getMapSize().height * 32;
    float ratio = visibleHeight / map_height;
    
    player = new Player(this, ratio);
    
    map->setPosition(Vec2(map->getMapSize().height * ratio, map->getMapSize().height * ratio));
    map->setScale(ratio);
   
    addChild(map);
    
    
    // Add map layers  - conversion into sprites with physics
    auto layer = map->getLayer("Obstacles");
    obstacle.CreateObstacle(this, layer, OBSTACLE_COLLISION_BITMASK);
    
    layer = map->getLayer("Collection");
    obstacle.CreateObstacle(this, layer, COLLECTION_COLLISION_BITMASK);
    
    layer = map->getLayer("FinishLine");
    obstacle.CreateObstacle(this, layer, FINISH_COLLISION_BITMASK);
    
     //Add score to top right of screen
    __String *tempScore = __String::createWithFormat("%i", score);

    scoreLabel = Label::createWithTTF( tempScore->getCString(), "Action Man.ttf", visibleHeight * 0.1);
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->setPosition(Point(visibleWidth - (scoreLabel->getContentSize().width / 2) - 10, visibleHeight - (scoreLabel->getContentSize().height / 2) - 10));

    this->addChild(scoreLabel,1000);
    
}

void GamePlay::GoToMainMenu(cocos2d::Ref *sender)
{
    auto scene = MainMenu::createScene();
    
    Director::getInstance()->replaceScene(scene);
}

void GamePlay::GoToReplayLevel(cocos2d::Ref *sender)
{
    auto scene = GamePlay::createScene(stageLoaded, levelLoaded);
    
    Director::getInstance()->replaceScene(scene);
}













