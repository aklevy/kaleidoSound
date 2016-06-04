#include "shapes.hpp"


Shapes::Shapes(World3D& world,std::shared_ptr<Node> parentNode, string typeName, ofMesh mesh, int ambiType)
    : _world(world),
      _parentNode(parentNode),
      _shapeType(typeName),
      _mesh(mesh)
{
    // Param Group setup
    _paramGroup.setName(typeName);

    // number of Shapes
    _paramGroup.add(_nbShapes.setup(parentNode,"Nb"+typeName,100,0,1000));
    _nbShapes.getAddress()->addCallback([&](const Value *v){
        Int * val= (Int *)v;
        if(val->value != _nbShapes){
            _nbShapes.set(val->value);
        }
    });

    _nbShapes.addListener(&_nbShapes,&Parameter<int>::listen);
    _nbShapes.addListener(this,&Shapes::nbChanged);

   /* for (unsigned i = 0; i <  _nbShapes; ++i)
    {
        _vecPos.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
       // _vecCol.push_back(ofColor::fromHsb(255 * i / (float) _nbShapes, 255, 255, 255));
    }*/


    // colorAmbience
    _paramGroup.add(_colorWarmCool.setup(parentNode,"CoolToWarm"+typeName,ambiType,0,100));
   _colorWarmCool.getAddress()->addCallback([&](const Value *v){
        Int * val= (Int *)v;
        if(val->value != _colorWarmCool){
           _colorWarmCool.set(val->value);
        }
    });

    _colorWarmCool.addListener(&_colorWarmCool,&Parameter<int>::listen);
    _colorWarmCool.addListener(&_colorAmbi,&ColorAmbience::changeAmbience);
    _colorAmbi.setup(_nbShapes, _colorWarmCool);

    _bDraw = true;


}
//---------------------------------------------------------
Shapes::~Shapes(){
    _nbShapes.removeListener(&_nbShapes,&Parameter<int>::listen);
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
            _particles.push_back(
                        _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                            ofRandom(20,35))->makeFree()); // create a node in top left back and fix
          }

}

//---------------------------------------------------------
void Shapes::update()
{

}

//---------------------------------------------------------
void Shapes::draw()
{
    if(_bDraw && _particles.size() == _nbShapes)
    {
        for (unsigned i = 0; i < _nbShapes; ++i)
        {
            ofSetColor(_colorAmbi.getColor(i));//_vecCol[i]);
            ofPushMatrix();
            ofTranslate(_particles[i]->getPosition());//_vecPos[i]);
            _mesh.draw();
            ofPopMatrix();
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
            _particles.push_back(
                        _world.makeParticle(ofVec3f(ofRandom(-width/2, width/2), ofRandom(-height, height), ofRandom(-width/2,width/2)),
                                            ofRandom(20,35))->makeFree()); // create a node in top left back and fix

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
