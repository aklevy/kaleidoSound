#include "ofApp.hpp"

ofApp::ofApp():
    _nw(),
    _world(World3D::create()),
    _boxes(Shapes(*_world,cam ,_nw.getSceneNode(),"Boxes",ofMesh::box(20, 20, 20))),
    _balls(Shapes(*_world,cam ,_nw.getSceneNode(),"Balls",ofMesh::sphere(20)))
{

}
//---------------------------------------------------------------------

ofApp::~ofApp () {
    _nw.setSimRunning(false);

}
//---------------------------------------------------------------------
void ofApp::setupGui()
{
    _gui.setup("Gui");
    _gui.setPosition(ofGetWidth()/2 , 0);

    // add reset button
    _gui.add(_reset.setup(_nw.getSceneNode(),"reset",false));
    _reset.getAddress()->addCallback([&](const Value *v){
        Bool * val= (Bool *)v;
        if(val->value != _reset){
            _reset.set(val->value);
        }
    });

    _reset.addListener(&_reset,&Parameter<bool>::listen);
    _reset.addListener(this,&ofApp::reset);

    // add FX
    _gui.add(_kSegments.setup(_nw.getSceneNode(),"kSegments",6,0,20));
    _kSegments.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _kSegments){
            _kSegments.set(val->value);
        }
    });

    _kSegments.addListener(&_kSegments,&Parameter<float>::listen);
    _kSegments.addListener(this,&ofApp::kaleidoSegmChanged);


    // add Physics' parameters to gui
    _gui.add(_physicsParam);

    // add Camera's parameters to gui
    _gui.add(_camParam);


    // add shapes' parameters to gui
    _gui.add(_boxes.getParamGroup());
    _gui.add(_balls.getParamGroup());


}
//---------------------------------------------------------------------
void ofApp::setupCamera()
{
    // add camera dx
    _camParam.setName("Camera");
    _camParam.add(_camDx.setup(_nw.getSceneNode(),"camDx",0,-1.0,1.0));
    _camDx.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _camDx){
           _camDx.set(val->value);
        }
    });
    _camDx.addListener(&_camDx,&Parameter<float>::listen);
    _camDx.addListener(this,&ofApp::moveCamDx);

    // add camera dy
    _camParam.add(_camDy.setup(_nw.getSceneNode(),"camDy",0,-50,50));
    _camDy.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _camDy){
           _camDy.set(val->value);
        }
    });

    _camDy.addListener(&_camDy,&Parameter<float>::listen);
    _camDy.addListener(this,&ofApp::moveCamDy);

    // add camera dz
    _camParam.add(_camDz.setup(_nw.getSceneNode(),"camDz",0,-100.0,100.0));
    _camDz.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _camDz){
           _camDz.set(val->value);
        }
    });

    _camDz.addListener(&_camDz,&Parameter<float>::listen);
    _camDz.addListener(this,&ofApp::moveCamDz);


}

//---------------------------------------------------------------------
void ofApp::setupWorld()
{

    // gravity
    _physicsParam.add(_gravity.setup(_nw.getSceneNode(),"Gravity",0.5,-10,10));
    _gravity.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _gravity){
            _gravity.set(val->value);
        }
    });

    _gravity.addListener(&_gravity,&Parameter<float>::listen);
    _gravity.addListener(this,&ofApp::gravityChanged);


    // particles bounce parameter
    _physicsParam.add(_particlesBounce.setup(_nw.getSceneNode(),"particlesBounce",1,0,1));
    _particlesBounce.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _particlesBounce){
            _particlesBounce.set(val->value);
        }
    });

   _particlesBounce.addListener(&_particlesBounce,&Parameter<float>::listen);
    _particlesBounce.addListener(this,&ofApp::bounceChanged);


    _world->setGravity(ofVec3f(0, _gravity, 0));

    // set world dimensions, not essential, but speeds up collision
    int width   = ofGetWidth();
    int height  = ofGetHeight();
    _world->setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));
   // _world->setSectorCount(SECTOR_COUNT);
    //_world->setDrag(0.97f);

    _world->setDrag(1);		// FIXTHIS
    _world->enableCollision();
    _world->setTimeStep(1.);
    _world->setNumIterations(10.);

}

//---------------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(0);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
   // ofSetCoordHandedness(OF_RIGHT_HANDED);


    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<DofPass>()->setEnabled(true);
    _kaleido = post.createPass<KaleidoscopePass>();
    _kaleido->setEnabled(false);

    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    

    // setup physics param group
    _physicsParam.setName("Physics");
    // Setup light
    light.setPosition(1000, 1000, 2000);

    _physicsParam.add(_bLight.setup(_nw.getSceneNode(),"Light",false));
    _bLight.getAddress()->addCallback([&](const Value *v){
        Bool * val= (Bool *)v;
        if(val->value != _bLight){
            _bLight.set(val->value);
        }
    });

    _bLight.addListener(&_bLight,&Parameter<bool>::listen);


    // create our own box mesh as there is a bug with
    // normal scaling and ofDrawBox() at the moment
    //boxMesh = ofMesh::box(20, 20, 20);

    // setup physics
    setupWorld();

    // setup camera params
    setupCamera();

   // cam.disableMouseInput();

    // create Particles
    _boxes.setup();
    _balls.setup();

    // Setup Gui communicating using Minuit with i-score
    setupGui();
}

//---------------------------------------------------------------------
void ofApp::reset(bool& newVal)
{
   if(newVal)
   {
       _boxes.reset();
        _balls.reset();
   }

   cam.setPosition(0,0,0);
    _reset.set(false);

}

//---------------------------------------------------------------------

void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    _world->update();
   // _boxes.update();
   // _balls.update();
}
//---------------------------------------------------------------------

void ofApp::draw()
{
    // copy enable part of gl state
    if(_bLight)
    {
        glPushAttrib(GL_ENABLE_BIT);
    
        // setup gl state
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        light.enable();
    }
    // begin scene to post process
    post.begin(cam);

    // draw Shapes
   _boxes.draw();
   _balls.draw();
    
   // ofDrawAxis(100);
    
    // end scene and draw
    post.end();
    
    // set gl state back to original
    if(_bLight)
        glPopAttrib();
    
    // draw GUI
    _gui.draw();
//ofDrawBitmapString("position"+mousepos,10,20);
    // draw help
   /* ofSetColor(0, 255, 255);
    ofDrawBitmapString("Number keys toggle effects, mouse rotates scene", 10, 20);
    for (unsigned i = 0; i < post.size(); ++i)
    {
        if (post[i]->getEnabled()) ofSetColor(0, 255, 255);
        else ofSetColor(255, 0, 0);
        ostringstream oss;
        oss << i << ": " << post[i]->getName() << (post[i]->getEnabled()?" (on)":" (off)");
        ofDrawBitmapString(oss.str(), 10, 20 * (i + 2));
    }*/
}
//---------------------------------------------------------------------

void ofApp::keyPressed(int key)
{
    if(key == ' ')
    {
        ofToggleFullscreen();
    }
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
}
//---------------------------------------------------------------------

void ofApp::mousePressed(int x, int y, int button)
{
}

//---------------------------------------------------------------------

void ofApp::gravityChanged(float &newVal)
{
    _world->setGravity(ofVec3f(0, newVal, 0));
}
//---------------------------------------------------------------------

void ofApp::kaleidoSegmChanged(float &newVal)
{
    _kaleido->setSegments(newVal);
}
//---------------------------------------------------------
void ofApp::bounceChanged(float &newBounce)
{
    for (auto p : _world->getParticles())
    {
        p->setBounce(newBounce);
        //p->setDrag(0);
    }

}
//---------------------------------------------------------
void ofApp::moveCamDx(float &newDx)
{
    ofVec3f newpos = cam.getPosition() + ofVec3f(newDx,0,0);
    cam.setPosition(newpos);

}
//---------------------------------------------------------
void ofApp::moveCamDy(float &newDy)
{
    ofVec3f newpos = cam.getPosition() + ofVec3f(0,newDy,0);
    cam.setPosition(newpos);

}
//---------------------------------------------------------
void ofApp::moveCamDz(float &newDz)
{
    ofVec3f newpos = cam.getPosition() + ofVec3f(0,0,newDz);
  //  cam.setPosition(newpos);
    cam.move(ofVec3f(0,0,newDz));
}
