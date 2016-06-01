#include "ofApp.hpp"

ofApp::ofApp():
    _nw(),
    _world(World3D::create()),
    _boxes(Shapes(*_world,_nw.getSceneNode(),"Boxes",ofMesh::box(20, 20, 20))),
    _balls(Shapes(*_world,_nw.getSceneNode(),"Balls",ofMesh::sphere(20)))
{

}
//---------------------------------------------------------------------

ofApp::~ofApp () {
    _nw.setSimRunning(false);

}
//---------------------------------------------------------------------
void ofApp::setupGui()
{
    _gui.setup("ShapesGui");
    _gui.setPosition(ofGetWidth()/2 , 0);

    // add Physics' parameters to gui
    _gui.add(_physicsParam);

    // add shapes' parameters to gui
    _gui.add(_boxes.getParamGroup());
    _gui.add(_balls.getParamGroup());


}
//---------------------------------------------------------------------
void ofApp::setupWorld()
{
    _physicsParam.setName("Physics");
    // gravity
    _physicsParam.add(_gravity.setup(_nw.getSceneNode(),"Gravity",1,-10,10));
    _gravity.getAddress()->addCallback([&](const Value *v){
        Float * val= (Float *)v;
        if(val->value != _gravity){
            _gravity.set(val->value);
        }
    });

    _gravity.addListener(&_gravity,&Parameter<float>::listen);
    _gravity.addListener(this,&ofApp::gravityChanged);

    _world->setGravity(ofVec3f(0, _gravity, 0));

    // set world dimensions, not essential, but speeds up collision
    int width   = ofGetWidth();
    int height  = ofGetHeight();
    _world->setWorldSize(ofVec3f(-width/2, -height, -width/2), ofVec3f(width/2, height, width/2));
   // _world->setSectorCount(SECTOR_COUNT);
    //_world->setDrag(0.97f);
    _world->setDrag(1);		// FIXTHIS
    _world->enableCollision();


}

//---------------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    ofSetCoordHandedness(OF_RIGHT_HANDED);


    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<DofPass>()->setEnabled(false);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    

    // Setup light
    light.setPosition(1000, 1000, 2000);
    

    // create our own box mesh as there is a bug with
    // normal scaling and ofDrawBox() at the moment
    //boxMesh = ofMesh::box(20, 20, 20);

    // setup physics
    setupWorld();

    // create Particles
    _boxes.setup();
    _balls.setup();

    // Setup Gui communicating using Minuit with i-score
    setupGui();
}

//---------------------------------------------------------------------

void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    _world->update();
}
//---------------------------------------------------------------------

void ofApp::draw()
{
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    // setup gl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    light.enable();
    
    // begin scene to post process
    post.begin(cam);
    
    // draw Shapes
    _boxes.draw();
    _balls.draw();
    
    ofDrawAxis(100);
    
    // end scene and draw
    post.end();
    
    // set gl state back to original
    glPopAttrib();
    
    // draw GUI
    _gui.draw();

    // draw help
    ofSetColor(0, 255, 255);
    ofDrawBitmapString("Number keys toggle effects, mouse rotates scene", 10, 20);
    for (unsigned i = 0; i < post.size(); ++i)
    {
        if (post[i]->getEnabled()) ofSetColor(0, 255, 255);
        else ofSetColor(255, 0, 0);
        ostringstream oss;
        oss << i << ": " << post[i]->getName() << (post[i]->getEnabled()?" (on)":" (off)");
        ofDrawBitmapString(oss.str(), 10, 20 * (i + 2));
    }
}
//---------------------------------------------------------------------

void ofApp::keyPressed(int key)
{
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
}
//---------------------------------------------------------------------

void ofApp::gravityChanged(float &newVal)
{
    _world->setGravity(ofVec3f(0, _gravity, 0));
}
