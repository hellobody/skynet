#ifndef __NEURON_H__
#define __NEURON_H__

#include "cocos2d.h"

class Link;

class Neuron : public cocos2d::Sprite {

    float excitement = 0.f;

    std::vector <Link *> vLinks;

public:

    Neuron ();
    virtual ~Neuron ();

    static Neuron *create ();
    static Neuron *createHexagon ();

    virtual void update (float);

    void connect (Link *);

    void reshapeLinks ();

    void excite (float);
};

#endif
