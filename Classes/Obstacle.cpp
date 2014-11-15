#include "Obstacle.h"
#include "Definitions.h"

USING_NS_CC;

Obstacle::Obstacle()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
}

void Obstacle::CreateObstacle( cocos2d::Layer *layer, cocos2d::TMXLayer *mapLayer, unsigned int bitmask)
{
    auto layerSize = mapLayer->getLayerSize();
    
    for (int y = 0; y < layerSize.height; y++)
    {
        for (int x = 0; x < layerSize.width; x++)
        {
            
            auto sprite = mapLayer->getTileAt(Point(x, y));
            if(sprite)
            {
                // apply collision masks and physics bodies to each sprite that is created from the tiles.
                
                
                auto spriteBody = PhysicsBody::createEdgeBox(sprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1);
                spriteBody->setCollisionBitmask( bitmask );
                spriteBody->setContactTestBitmask( false );
                spriteBody->setEnable(false);
                sprite->setPhysicsBody(spriteBody);
                
                layer->scheduleOnce( schedule_selector( Obstacle::EnableTilePhysics ), MOVEMENT_SPEED * visibleSize.width );
                
            }
        }
        
    }
}

void Obstacle::EnableTilePhysics(float dt){
    
}