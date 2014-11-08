#include "GamePlay.h"
#include "Obstacle.h"
#include "Definitions.h"

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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->Setup();  // Load the stage / level
    
    // play area
    visiblewidth = visibleSize.width;
    
    // setup bounding box
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
    edgeBody->setContactTestBitmask( true );
    
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);
    
    this->addChild(edgeNode);
    
    // MOVE TO PUFF CLASS
    
    // create puff player
    puff = Sprite::create("puff.png");
    auto puffBody = PhysicsBody::createCircle(puff->getContentSize().width / 2);
    puffBody->setCollisionBitmask( PUFF_COLLISION_BITMASK );
    puffBody->setContactTestBitmask( true );
    puff->setPhysicsBody(puffBody);
    puff->setPosition(Point(visibleSize.width / 4 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(puff);
    
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
        mapAction = MoveBy::create(MOVEMENT_SPEED * visiblewidth, Point(-visiblewidth * 1.5, 0));
        map->runAction(mapAction);
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
    
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CCLOG("Collision Occured");
        GamePlay::stopAction(mapAction);
        
    }
    
    if( ( PUFF_COLLISION_BITMASK == a->getCollisionBitmask() &&  0x000004 == b->getCollisionBitmask() ) || (PUFF_COLLISION_BITMASK == b->getCollisionBitmask() && 0x000004 == a->getCollisionBitmask() ) ) //Definitions file definition was not working, consult with Sean.
    {
       // collectibleLayer->removeTileAt(Point(collectionSprite->getPositionX(),collectionSprite->getPositionY()));
        // in order to make this work I need to get the coordinates of the tiles, which I cannot do for some reason.
        
    }

    
    
    
    return true;
}

void GamePlay::Setup()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    __String *level = __String::createWithFormat("tiles/%i_%i.tmx", stageLoaded,levelLoaded);
    map = TMXTiledMap::create(level->getCString());
    map->setAnchorPoint(Vec2(0, 0));

    float map_height = map->getMapSize().height * 32;
    float ratio = visibleSize.height / map_height;
    map->setScale(ratio);
    
   
    addChild(map);
    
    // move the map
//    mapAction = MoveBy::create(MOVEMENT_SPEED * visibleSize.width, Point(-visibleSize.width * 1.5, 0));
//    map->runAction(mapAction);


    
    // creating top and bottom obstacle layer
    
    auto layer = map->getLayer("Obstacles");
    auto layerSize = layer->getLayerSize();
    

    
    for (int y = 0; y < layerSize.height; y++)
    {
        for (int x = 0; x < layerSize.width; x++)
        {

            auto sprite = layer->getTileAt(Point(x, y));
            if(sprite)
            {
                // apply collision masks and physics bodies to each sprite that is created from the tiles.
                auto spriteBody = PhysicsBody::createEdgeBox(sprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1);
                spriteBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
                spriteBody->setContactTestBitmask( true );
                sprite->setPhysicsBody(spriteBody);
                
                
                obstacle.CreateObstacle(this, sprite);

                
            }

        }
    }

    // creating a collision layer
    auto collisionLayer = map->getLayer("Collision");
    auto collisionLayerSize = collisionLayer->getLayerSize();
    
    
    
    for (int y = 0; y < collisionLayerSize.height; y++)
    {
        for (int x = 0; x < collisionLayerSize.width; x++)
        {
            
            auto collisionSprite = collisionLayer->getTileAt(Point(x, y));
            if(collisionSprite)
            {
                // apply collision masks and physics bodies to each sprite that is created from the tiles.
                auto collisionSpriteBody = PhysicsBody::createEdgeBox(collisionSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1);
                collisionSpriteBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
                collisionSpriteBody->setContactTestBitmask( true );
                collisionSprite->setPhysicsBody(collisionSpriteBody);
                
                
                obstacle.CreateObstacle(this, collisionSprite);
                
                
            }
            
        }
    }


    // creating a collectible layer
    auto collectibleLayer = map->getLayer("Collectible");
    auto collectibleLayerSize = collectibleLayer->getLayerSize();
    
    
    
    for (int y = 0; y < collectibleLayerSize.height; y++)
    {
        for (int x = 0; x < collectibleLayerSize.width; x++)
        {
            
            auto collectionSprite = collectibleLayer->getTileAt(Point(x, y));
            if(collectionSprite)
            {
                // apply collision masks and physics bodies to each sprite that is created from the tiles.
                auto collectionSpriteBody = PhysicsBody::createEdgeBox(collectionSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1);
                collectionSpriteBody->setCollisionBitmask( 0x000004 );
                collectionSpriteBody->setContactTestBitmask( true );
                collectionSprite->setPhysicsBody(collectionSpriteBody);
                
                
                obstacle.CreateObstacle(this, collectionSprite);
                
            }
            
        }
    }

    
    
    
}













