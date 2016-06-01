#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxGui.h"
#include "MSAPhysics3D.h"

#include "network.hpp"
#include "shapes.hpp"
//#include "parameter.hpp"
using namespace msa::physics;

class ofApp : public ofBaseApp
{
public:

    ofApp();
    ~ofApp();

    void setupGui();
    void setupWorld();
    void setup();


    void update();
    void draw();

    void keyPressed(int key);
    void gravityChanged(float &newVal);
    
private:

    // gui
    ofxPanel            _gui;

    // Network for communication
    Network             _nw;

    // scene stuff
    ofxPostProcessing   post;
    ofEasyCam           cam;
    ofLight             light;

    //physics
    World3D_ptr         _world;
    ofParameterGroup    _physicsParam;
    Parameter<float>    _gravity;

    // Shapes
    Shapes              _boxes;
    Shapes              _balls;




};
