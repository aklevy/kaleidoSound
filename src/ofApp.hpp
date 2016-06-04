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
    void reset(bool &newVal);

    void update();
    void draw();

    void keyPressed(int key);
    void gravityChanged(float &newVal);
    void kaleidoSegmChanged(float &newVal);
    
private:

    // gui
    ofxPanel            _gui;
    Parameter<bool>     _reset;
    // Network for communication
    Network             _nw;

    // scene stuff
    ofEasyCam           cam;
    ofLight             light;

    // FX
    ofxPostProcessing   post;
    shared_ptr<KaleidoscopePass> _kaleido;
    Parameter<float>    _kSegments;


    //physics
    World3D_ptr         _world;
    ofParameterGroup    _physicsParam;
    Parameter<float>    _gravity;
    Parameter<bool>     _bLight;

    // Shapes
    Shapes              _boxes;
    Shapes              _balls;




};
