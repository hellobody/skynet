#ifndef __LINK_H__
#define __LINK_H__

#include "cocos2d.h"

class Neuron;

class Link : public cocos2d::Sprite {

    float bandwidth = .49f;
    float length = 0;

    Neuron *neuronA = nullptr;
    Neuron *neuronB = nullptr;

public:

    Link ();
    virtual ~Link ();

    static Link *create ();

    virtual void update (float);

    void connectA (Neuron *);
    void connectB (Neuron *);

    void reshape ();

    void sendSignal (Neuron *sender, float signal_value);
    void inhibit ();
    void promote ();
};

#endif
