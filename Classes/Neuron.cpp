#include "Neuron.h"
#include "Link.h"

USING_NS_CC;
Neuron::Neuron () {}
Neuron::~Neuron () {}

Neuron *Neuron::create () {

    Neuron *pNeuron = new Neuron ();

    if (pNeuron->initWithFile ("neuron.png")) {
        pNeuron->autorelease ();
        return pNeuron;
    }

    CC_SAFE_DELETE (pNeuron);
    return NULL;
}

Neuron *Neuron::createPositive () {

    Neuron *pNeuron = new Neuron ();

    if (pNeuron->initWithFile ("neuron_positive.png")) {
        pNeuron->autorelease ();
        pNeuron->type = ePostitive;
        return pNeuron;
    }

    CC_SAFE_DELETE (pNeuron);
    return NULL;
}

Neuron *Neuron::createNegative () {

    Neuron *pNeuron = new Neuron ();

    if (pNeuron->initWithFile ("neuron_negative.png")) {
        pNeuron->autorelease ();
        pNeuron->type = eNegative;
        return pNeuron;
    }

    CC_SAFE_DELETE (pNeuron);
    return NULL;
}

Neuron *Neuron::createHexagon () {

    Neuron *pNeuron = new Neuron ();

    if (pNeuron->initWithFile ("hexagon.png")) {
        pNeuron->autorelease ();
        return pNeuron;
    }

    CC_SAFE_DELETE (pNeuron);
    return NULL;
}

void Neuron::update (float dt) {

    if (excitement > 0.f) {
        excitement -= dt;
        if (excitement < 0.f) {
            excitement = 0.f;
        }
    }
    
    setColor (Color3B (int (255 * excitement), int (255 * (1.f - excitement)), int (255 * (1.f - excitement))));
}

void Neuron::connect (Link *link) { vLinks.push_back (link); }

void Neuron::reshapeLinks () {
    for (Link *link : vLinks) { link->reshape (); }
}

void Neuron::excite (float v, Link *exciting_link) {

    if (v < .1f) {
        return; // this should prevent stack overflow
    }

    if (excitement > v) { // do not excite if already excited
        return;
    };

    excitement = v;

    if (excitement > 1.f) {
        excitement = 1.f;
    }

    if (exciting_link) {
        if (type == ePostitive) {
            exciting_link->promote ();
        } else if (type == eNegative) {
            exciting_link->inhibit ();
        }
    }

    if (excitement > 0.f) {
        for (Link *link : vLinks) {
            if (exciting_link == nullptr || exciting_link != link) {
                link->sendSignal (this, excitement);
            }
        }
    }
}
