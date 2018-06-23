#include "Link.h"
#include "Neuron.h"

USING_NS_CC;
Link::Link () {}
Link::~Link () {}

Link *Link::create () {

    Link *pSprite = new Link ();

    if (pSprite->initWithFile ("link.png")) {
        pSprite->autorelease ();
        return pSprite;
    }

    CC_SAFE_DELETE (pSprite);
    return NULL;
}

void Link::update (float dt) {
    setScaleY (bandwidth);
    setScaleX (length);
}

void Link::connectA (Neuron *neuron) {
    neuron->connect (this);
    neuronA = neuron;
    reshape ();
}

void Link::connectB (Neuron *neuron) {
    neuron->connect (this);
    neuronB = neuron;
    reshape ();
}

void Link::reshape () {
    if (neuronA && neuronB) {
        Vec2 pos = (neuronA->getPosition () + neuronB->getPosition ()) / 2;
        Vec2 dir = neuronA->getPosition () - neuronB->getPosition ();
        setRotation (-CC_RADIANS_TO_DEGREES (atan2f (dir.y, dir.x)));
        length = dir.length ();
        setPosition (pos);
        update (0); // to apply scale
    }
}
