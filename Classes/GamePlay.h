#ifndef __GAME_PLAY_H__
#define __GAME_PLAY_H__

#include "cocos2d.h"

class GamePlay : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene( unsigned int stageRef, unsigned int levelRef );
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GamePlay);
    
private:
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ){ sceneWorld = world; };
    
    bool onContactBegin( cocos2d::PhysicsContact &contact );
    
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
    void onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *event );
    
    void Setup();
    
    cocos2d::PhysicsWorld *sceneWorld;
    
    cocos2d::Sprite *puff;

};

#endif // __STAGE__H__
