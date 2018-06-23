#include "SpaceScene.h"
#include "SimpleAudioEngine.h"
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace cocos2d::ui;

Scene *Space::createScene () { return Space::create (); }

bool Space::init () {
    if (!Scene::init ())
        return false;

    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();

    const float buttonStepY = 80.f;

    auto buttonPlay = Button::create ("button_normal.png", "button_pressed.png", "button_pressed.png");
    buttonPlay->setPosition (Vec2 (origin.x + buttonPlay->getBoundingBox ().size.width / 2 + 10, origin.y + visibleSize.height - buttonStepY));
    buttonPlay->setTitleText ("Play");
    buttonPlay->setTitleFontSize (32);
    buttonPlay->addTouchEventListener ([&] (Ref *sender, Widget::TouchEventType type) { if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG ("pressed");
    }});
    addChild (buttonPlay, 2);

    auto buttonReset = Button::create ("button_normal.png", "button_pressed.png", "button_pressed.png");
    buttonReset->setPosition (Vec2 (origin.x + buttonPlay->getBoundingBox ().size.width / 2 + 10, origin.y + visibleSize.height - buttonStepY * 2));
    buttonReset->setTitleText ("Reset");
    buttonReset->setTitleFontSize (32);
    buttonReset->addTouchEventListener ([&] (Ref *sender, Widget::TouchEventType type) { if (type == ui::Widget::TouchEventType::ENDED) {
        for (auto child : vHexagons) {
            removeChild (child);
        }
        for (auto child : vNeurons) {
            removeChild (child);
        }
        for (auto child : vLinks) {
            removeChild (child);
        }
        vHexagons.clear ();
        vNeurons.clear ();
        vLinks.clear ();
        buildMatrix (12);
        createLayers (7);
    }});
    addChild (buttonReset, 2);

    timeCounterForScale = halfPi;

    auto touchEventListener = EventListenerTouchOneByOne::create ();
    touchEventListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        for (Neuron *neuron : vHexagons) {
            if (neuron->getBoundingBox ().containsPoint (touch->getLocation ())) {
                if (spaceKeyPressed) {
                    neuron->excite (1.f);
                    return true;
                }
            }
        }

        for (Neuron *neuron : vNeurons) {
            if (neuron->getBoundingBox ().containsPoint (touch->getLocation ())) {

                if (spaceKeyPressed) {
                    neuron->excite (1.f);
                    return true;
                }

                if (selectedNeuron) {
                    if (selectedNeuron == neuron) { // deselecting neuron if clicked again
                        selectedNeuron->setScale (1.f);
                        selectedNeuron = nullptr;
                    } else { // deselecting previously selected neuron and selecting new one
                        if (leftCtrlKeyPressed) {
                            Link *link = Link::create ();
                            link->connectA (selectedNeuron);
                            link->connectB (neuron);
                            vLinks.push_back (link);
                            addChild (link, 0);
                        } else {
                            selectedNeuron->setScale (1.f);
                            selectedNeuron = neuron;
                            timeCounterForScale = halfPi;
                        }
                    }
                } else {
                    selectedNeuron = neuron;
                    timeCounterForScale = halfPi;
                }
                return true;
            }
        }

        // if we didn't touch any existing neuron then creating new one
        if (leftCtrlKeyPressed == false) {
            Neuron *neuron = Neuron::create ();
            neuron->setPosition (touch->getLocation ());
            vNeurons.push_back (neuron);
            addChild (neuron, 1);
        }
        return true;
    };
    touchEventListener->onTouchMoved = [=] (Touch *touch, Event *event) {
        for (Neuron *neuron : vHexagons) {
            if (neuron->getBoundingBox ().containsPoint (touch->getLocation ())) {
                if (spaceKeyPressed) {
                    neuron->excite (1.f);
                    break;
                }
            }
        }

        if (takenNeuron) {
            takenNeuron->setPosition (touch->getLocation ());
            takenNeuron->reshapeLinks ();
        } else if (selectedNeuron && selectedNeuron->getBoundingBox ().containsPoint (touch->getLocation ())) {
            takenNeuron = selectedNeuron;
            selectedNeuron = nullptr;
        }
    };
    touchEventListener->onTouchEnded = [=] (Touch *touch, Event *event) {
        if (takenNeuron) {
            takenNeuron->setScale (1.f);
            takenNeuron = nullptr;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority (touchEventListener, this);

    // android back press event
    auto keyboardListener = EventListenerKeyboard::create ();
    keyboardListener->onKeyPressed = [=] (cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_CTRL) { leftCtrlKeyPressed = true; }
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) { spaceKeyPressed = true; }
    };
    keyboardListener->onKeyReleased = [=] (cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) { Director::getInstance ()->end (); } // exiting application
        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_CTRL) { leftCtrlKeyPressed = false; }
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) { spaceKeyPressed = false; }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority (keyboardListener, this);

    buildMatrix (5);
    //createLayers (7);

    scheduleUpdate ();
    return true;
}

void Space::update (float dt) {

    if (selectedNeuron || takenNeuron) {
        timeCounterForScale += dt * 10;
        float scale = 1.f + cos (timeCounterForScale) * .25;
        if (selectedNeuron) selectedNeuron->setScale (scale);
        if (takenNeuron) takenNeuron->setScale (scale);
    } else { timeCounterForScale = halfPi; }

    for (Neuron *hexagon : vHexagons) { hexagon->update (dt); }
    for (Neuron *neuron : vNeurons) { neuron->update (dt); }
    for (Link *link : vLinks) { link->update (dt); }
}

void Space::buildMatrix (int size) {

    const int doubleSize = size * 2;
    const float stepX = 28.f;
    const float stepY = 16.f;
    const float doubleStepY = stepY * 2;
    const Vec2 startPoint (stepX/2 + 6, doubleStepY * (size-1) + stepY * size + 4);

    for (int i=0; i<size; i++) {
        for (int j=0; j<size+i; j++) {
            Neuron *hexagon = Neuron::createHexagon ();
            hexagon->setPosition (startPoint + Vec2 (j * stepX, j * stepY - i * doubleStepY));
            vHexagons.push_back (hexagon);
            addChild (hexagon, 2);
        }
    }
    for (int i=size; i<doubleSize-1; i++) {
        for (int j=i-size+1; j<doubleSize-1; j++) {
            Neuron *hexagon = Neuron::createHexagon ();
            hexagon->setPosition (startPoint + Vec2 (j * stepX, j * stepY - i * doubleStepY));
            vHexagons.push_back (hexagon);
            addChild (hexagon, 2);
        }
    }
}

void Space::createLayers (int n) {

    const float stepY = 37.f;
    const float stepX = 60.f;

    for (int i=0; i < (int) vHexagons.size (); i++) {

        Neuron *neuron = Neuron::create ();
        neuron->setPosition (300, 700 - i * stepY);
        vNeurons.push_back (neuron);
        addChild (neuron, 1);

        Link *link = Link::create ();
        link->connectA (vHexagons [i]);
        link->connectB (neuron);
        vLinks.push_back (link);
        addChild (link, 0);
    }

    for (int i=0; i < n - 1; i++) {
        for (int j=0; j < (int) vHexagons.size (); j++) {

            Neuron *neuron = Neuron::create ();
            neuron->setPosition (300 + (i + 1) * stepX, 700 - j * stepY);
            vNeurons.push_back (neuron);
            addChild (neuron, 1);

            Link *link = Link::create ();
            link->connectA (vNeurons [j + i * (int) vHexagons.size ()]);
            link->connectB (neuron);
            vLinks.push_back (link);
            addChild (link, 0);
        }
    }
}
