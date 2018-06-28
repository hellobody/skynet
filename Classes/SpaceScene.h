#ifndef __SPACE_SCENE_H__
#define __SPACE_SCENE_H__

#include "cocos2d.h"
#include "Neuron.h"
#include "Link.h"

class Space : public cocos2d::Scene {

    std::vector <Neuron *> vHexagons;
    std::vector <Neuron *> vNeurons;
    std::vector <Link *> vLinks;

    Neuron *takenNeuron = nullptr; // taken to be moved
    Neuron *selectedNeuron = nullptr; // selected to be linked with another

    bool leftCtrlKeyPressed = false;
    bool spaceKeyPressed = false;
    bool p_key_pressed = false;
    bool n_key_pressed = false;
    bool random_exciting = false;

    const float halfPi = 1.5707f; // it's almost Pi / 2 to start taken values from 0 from cos func

    float timeCounterForScale;

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    void AddButtons ();
    void TouchEventsProcessing ();
    void KeyboardProcessing ();

public:
    static cocos2d::Scene *createScene ();

    virtual bool init ();

    void buildMatrix (int);
    void createLayers (int);

    void update (float) override;

    CREATE_FUNC (Space);
};

#endif
