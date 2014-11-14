#include "GamePlay.h"
#include "Obstacle.h"
#include "Definitions.h"
#include "Player.h"

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
    
    // MOVE TO PUFF CLASS
    
    // create puff player
//    puff = Sprite::create("puff.png");
//    auto puffBody = PhysicsBody::createCircle(puff->getContentSize().width / 2);
//    puffBody->setCollisionBitmask( PUFF_COLLISION_BITMASK );
//    puffBody->setContactTestBitmask( true );
//    puff->setPhysicsBody(puffBody);
//    puff->setPosition(Point(visibleSize.width / 4 + origin.x, visibleSize.height / 2 + origin.y));
   //    this->addChild(puff);
    
    puff = new Player(this);

    
    // setup physics
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( GamePlay::onContactBegin, this );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    //setup touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( GamePlay::onTouchBegan, this );
    touchListener->onTouchMoved = CC_CALLBACK_2( GamePlay::onTouchMoved, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, puff);
    
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
            mapAction = RepeatForever::create(MoveBy::create(MOVEMENT_SPEED * visibleWidth, Point(-visibleWidth * 1.5, 0)));            map->runAction(mapAction);
        }
        levelStarted = 1;
        
        return true;
    }
    return false;
}


void GamePlay::onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *event )
{
    Vec2 touchPos = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

    puff->setPosition(touchPos);
}

bool GamePlay::onContactBegin( cocos2d::PhysicsContact &contact )
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    // Obstacle Collision
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CCLOG("Collision");
        GamePlay::stopAction(mapAction);
        
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->getEventDispatcher()->release();
    }
    
    // Collection Collision
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && COLLECTION_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && COLLECTION_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CCLOG("Collection");
        b->setEnable(false);
    }
    
    // FinishLine Collision
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && FINISH_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && FINISH_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
         CCLOG("Finish");
        b->setEnable(false);
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
    
}














