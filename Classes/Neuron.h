#ifndef __NEURON_H__
#define __NEURON_H__

#include "cocos2d.h"

class Link;

class Neuron : public cocos2d::Sprite {

    enum ENeuronType {eNeutral, ePostitive, eNegative};

    float excitement = 0.f;

    std::vector <Link *> vLinks;

    ENeuronType type = eNeutral;

public:

    Neuron ();
    virtual ~Neuron ();

    static Neuron *create ();
    static Neuron *createPositive ();
    static Neuron *createNegative ();
    static Neuron *createHexagon ();

    virtual void update (float);

    void connect (Link *);

    void reshapeLinks ();

    void excite (float v, Link *exciting_link = nullptr);
};

#endif
