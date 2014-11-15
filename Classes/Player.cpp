#include "Player.h"
#include "Definitions.h"

USING_NS_CC;

Player::Player(cocos2d::Layer *layer, float scaleRatio)
{
    visibleSize = Director::getInstance( )->getVisibleSize( );
    origin = Director::getInstance( )->getVisibleOrigin( );
    
    //Create an position the sprite.
    puff = Sprite::create("puff.png");
    puff->setScale(scaleRatio);
    puff->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    //create a physics body that is a cirle with the radius of half the width of the ball that is flappybird sprite.
    auto puffBody = PhysicsBody::createCircle( puff->getContentSize().width/2);
    puffBody->setCollisionBitmask(PUFF_COLLISION_BITMASK);
    puffBody->setContactTestBitmask(true);
    
    puff->setPhysicsBody(puffBody);
    
    layer->addChild(puff, 100);
}

void Player::UpdatePosition(cocos2d::Vec2 vec2)
{
    puff->setPosition(vec2);
}

