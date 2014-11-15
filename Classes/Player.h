#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player
{
public:
    cocos2d::Sprite *puff;
    
    Player(cocos2d::Layer *sprite);
    
    void UpdatePosition(cocos2d::Vec2 vec2);
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    
};

#endif // __PLAYER_H__
