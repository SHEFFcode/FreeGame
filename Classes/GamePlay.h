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
    float numTilesPhysicsMoved;
    unsigned int score = 0;
    unsigned int levelStarted = 0;
    unsigned int enabled = 0;
    unsigned int numTilesPhysics = 0;
    unsigned int numTilesPhysicsStart = 0;
    unsigned int numTilesPhysicsRemovalKey = 0;
    
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
    void onTouchMoved( cocos2d::Touch *touch, cocos2d::Event *event );
    
    bool onContactBegin( cocos2d::PhysicsContact &contact );
    
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ){ sceneWorld = world; };
    void GoToReplayLevel(cocos2d::Ref *sender);
    void GoToMainMenu(cocos2d::Ref *sender);
    void EnableTilePhysics( float dt );
    void RemoveTilePhysics( float dt );
    
    cocos2d::Action *mapAction;
    cocos2d::TMXTiledMap *map;
    cocos2d::PhysicsWorld *sceneWorld;
    cocos2d::Label *scoreLabel;
    
    Player *player;
    
    Obstacle obstacle;
    
    cocos2d::TMXLayer *collectionLayer;
    
};

#endif // __STAGE__H__
