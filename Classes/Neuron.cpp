#include "Neuron.h"
#include "Link.h"

USING_NS_CC;
Neuron::Neuron () {}
Neuron::~Neuron () {}

Neuron *Neuron::create () {

    Neuron *pSprite = new Neuron ();

    if (pSprite->initWithFile ("neuron.png")) {
        pSprite->autorelease ();
        return pSprite;
    }

    CC_SAFE_DELETE (pSprite);
    return NULL;
}

Neuron *Neuron::createHexagon () {

    Neuron *pSprite = new Neuron ();

    if (pSprite->initWithFile ("hexagon.png")) {
        pSprite->autorelease ();
        return pSprite;
    }

    CC_SAFE_DELETE (pSprite);
    return NULL;
}

void Neuron::update (float dt) {
    setColor (Color3B (int (255 * (1.f - excitement)), int (255 * (1.f - excitement)), int (255 * excitement)));
}

void Neuron::connect (Link *link) { vLinks.push_back (link); }

void Neuron::reshapeLinks () {
    for (Link *link : vLinks) { link->reshape (); }
}

void Neuron::excite (float v) {
    excitement = v;
}
