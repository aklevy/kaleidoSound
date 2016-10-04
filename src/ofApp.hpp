#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxGui.h"
#include "MSAPhysics3D.h"

#include "ofxOssia.hpp"
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
    void setupCamera();

    void setup();
    void reset(bool &newVal);

    void update();
    void draw();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    // parameters' listeners
    void gravityChanged(float &newVal);
    void kaleidoSegmChanged(float &newVal);
    void bounceChanged(float &newBounce);
    void moveCamDx(float &newDx);
    void moveCamDy(float &newDy);
    void moveCamDz(float &newDz);

private:

    // gui
    ofxPanel            _gui;
    ossia::Parameter<bool>     _reset;
    // Network for communication
    Network             _nw;

    // scene stuff
    ofEasyCam           cam;
    ofParameterGroup    _camParam;
    ossia::Parameter<float>    _camDx;
    ossia::Parameter<float>    _camDy;
    ossia::Parameter<float>    _camDz;
ofVec2f mousepos;
    ofLight             light;

    // FX
    ofxPostProcessing   post;
    shared_ptr<KaleidoscopePass> _kaleido;
    ossia::Parameter<float>    _kSegments;


    //physics
    World3D_ptr         _world;
    ofParameterGroup    _physicsParam;
    ossia::Parameter<float>    _gravity;
    ossia::Parameter<bool>     _bLight;
    ossia::Parameter<float>    _particlesBounce;

    // Shapes
    Shapes              _boxes;
    Shapes              _balls;




};
