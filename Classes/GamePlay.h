#ifndef __GAME_PLAY_H__
#define __GAME_PLAY_H__

#include "cocos2d.h"
#include "Obstacle.h"
#include "Player.h"

class GamePlay : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene( unsigned int stageRef, unsigned int levelRef );
    
    virtual bool init();
    
    CREATE_FUNC(GamePlay);
    
private:
    float visibleWidth;
    float visibleHeight;
    
    unsigned int levelStarted = 0;
    
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
    void onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *event );
    
    bool onContactBegin( cocos2d::PhysicsContact &contact );
    
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ){ sceneWorld = world; };
    
    void Setup();
    
    cocos2d::Action *mapAction;
    cocos2d::TMXTiledMap *map;
    cocos2d::PhysicsWorld *sceneWorld;
    
    Player *player;
    
    Obstacle obstacle;
    
};

#endif // __STAGE__H__
