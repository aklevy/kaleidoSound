#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxGui.h"

#include "network.hpp"
#include "shapes.hpp"
//#include "parameter.hpp"

class ofApp : public ofBaseApp
{
public:

    ofApp();
    ~ofApp();

    void setup();
    void setupGui();

       void update();
    void draw();

    void keyPressed(int key);
    
private:

    // gui
    ofxPanel            _gui;

    // Network for communication
    Network _nw;

    // scene stuff
    ofxPostProcessing post;
    ofEasyCam cam;
    ofLight light;
    
    // Shapes
    Shapes      _boxes;
    Shapes      _balls;




};
