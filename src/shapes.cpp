#include "shapes.hpp"


Shapes::Shapes(World3D& world,ofEasyCam& cam,ossia::net::node_base& parentNode, string typeName, ofMesh mesh, int ambiType)
    : _world(world),
      _cam(cam),
      _parentNode(parentNode),
      _shapeType(typeName),
      _mesh(mesh)
{
    // Param Group setup
    _paramGroup.setName(typeName);

    // number of Shapes
    _paramGroup.add(_nbShapes.setup(parentNode,"Nb"+typeName,100,0,500));
    _nbShapes.addListener(this,&Shapes::nbChanged);

    /* for (unsigned i = 0; i <  _nbShapes; ++i)
    {
        _vecPos.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
       // _vecCol.push_back(ofColor::fromHsb(255 * i / (float) _nbShapes, 255, 255, 255));
    }*/


    // colorAmbience
    _paramGroup.add(_colorWarmCool.setup(parentNode,"CoolToWarm"+typeName,ambiType,0,100));
    _colorWarmCool.addListener(&_colorAmbi,&ColorAmbience::changeAmbience);
    _colorAmbi.setup(_nbShapes, _colorWarmCool);

    _bDraw = true;


}
//---------------------------------------------------------
Shapes::~Shapes()
{
    _nbShapes.removeListener(this,&Shapes::nbChanged);

}
//---------------------------------------------------------
void Shapes::setup()
{

    //Particle3D_ptr parti;
    int width   = ofGetWidth();
    int height  = ofGetHeight();
    for (unsigned i = 0; i <  _nbShapes; ++i)
    {
        if(_shapeType == "Boxes")
        {

            _particles.push_back(
                        _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                            0)); // create a node in top left back and fix
        }
        else
        {
            _particles.push_back(
                        _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                            ofRandom(20,35))->makeFree()); // create a node in top left back and fix
        }

    }

}

//---------------------------------------------------------
void Shapes::update()
{
    /*int count = 0;
    if(_bDraw && !_particles.empty() &&_particles.size() == _nbShapes)
    {

        _bInsideScreen.clear();
        for (unsigned i = 0; i < _nbShapes; ++i)
        {
            ofPoint screenPos =;

            if(screen.inside(screenPos.x,screenPos.y))
            {
               _bInsideScreen.push_back(true);
               count ++;
            }
            else
            {
              _bInsideScreen.push_back(false);
            }
        }
    }*/
}

//---------------------------------------------------------
void Shapes::draw()
{
    if(_bDraw && _particles.size() == _nbShapes)
    {
        ofRectangle screen(0,0,ofGetWidth(),ofGetHeight());
        if(_shapeType == "Boxes")
        {
            for (unsigned i = 0; i < _nbShapes; ++i)
            {
                if( screen.inside(_cam.worldToScreen(_particles[i]->getPosition())))
                {
                    ofSetColor(_colorAmbi.getColor(i));//_vecCol[i]);
                    ofPushMatrix();
                    ofTranslate(_particles[i]->getPosition());//_vecPos[i]);
                    ofDrawBox(20,20,20);
                    ofPopMatrix();

                    /*
                     ofPushMatrix();
                     ofTranslate(_particles[i]->getPosition());//_vecPos[i]);
                     _mesh.draw();
                     ofPopMatrix();*/
                }

            }
        }
        else
        {
            for (unsigned i = 0; i < _nbShapes; ++i)
            {
                if( screen.inside(_cam.worldToScreen(_particles[i]->getPosition())))
                {
                    ofSetColor(_colorAmbi.getColor(i));//_vecCol[i]);
                    ofDrawCircle(_particles[i]->getPosition(),20.);
                    /*
                     ofPushMatrix();
                     ofTranslate(_particles[i]->getPosition());//_vecPos[i]);
                     _mesh.draw();
                     ofPopMatrix();*/
                }

            }
        }

    }
}
//---------------------------------------------------------
void Shapes::reset()
{
    // kill all the particles
    for (auto parti : _particles)
    {

        parti->kill();
    }
    // clear vector
    _particles.clear();

    //add new particles
    int width   = ofGetWidth();
    int height  = ofGetHeight();
    for (unsigned i = 0; i <  _nbShapes; ++i)
    {
        if(_shapeType == "Boxes")
        {
            _particles.push_back(
                        _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                            0)); // create a node in top left back and fix
        }
        else
        {
            _particles.push_back(
                        _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                            ofRandom(20,35))->makeFree()); // create a node in top left back and fix
        }
    }
}

//---------------------------------------------------------
void Shapes::nbChanged(int &newNb)
{
    _bDraw = false;

    int diff = newNb - _particles.size();
    int initialSize = _particles.size();
    if(diff > 0 )
    {

        // add new particles
        //Particle3D_ptr parti;

        int width   = ofGetWidth();
        int height  = ofGetHeight();
        for (unsigned i = 0; i <  diff; ++i)
        {
            if(_shapeType == "Boxes")
            {
                _particles.push_back(
                            _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                                0)); // create a node in top left back and fix
            }
            else
                _particles.push_back(
                            _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                                ofRandom(20,35))->makeFree()); // create a node in top left back and fix

        }
    }
    else if(diff < 0 )
    {
        // kill some particles
        for (unsigned i = initialSize - abs(diff); i <  initialSize; ++i)
        {
            _particles[i]->kill();
            _particles.pop_back();
        }
    }

    _colorAmbi.updateVectorSize(newNb);

    _bDraw = true;
}

