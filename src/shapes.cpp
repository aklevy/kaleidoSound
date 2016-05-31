#include "shapes.hpp"


Shapes::Shapes(std::shared_ptr<Node> parentNode, string typeName, ofMesh mesh, int ambiType)
    :_parentNode(parentNode),
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

    for (unsigned i = 0; i <  _nbShapes; ++i)
    {
        _vecPos.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
       // _vecCol.push_back(ofColor::fromHsb(255 * i / (float) _nbShapes, 255, 255, 255));
    }


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
void Shapes::update()
{

}

//---------------------------------------------------------
void Shapes::draw()
{
    if(_bDraw)
    {
        for (unsigned i = 0; i < _nbShapes; ++i)
        {
            ofSetColor(_colorAmbi.getColor(i));//_vecCol[i]);
            ofPushMatrix();
            ofTranslate(_vecPos[i]);
            _mesh.draw();
            ofPopMatrix();
        }
    }
}
//---------------------------------------------------------
void Shapes::nbChanged(int &newNb)
{
    _bDraw = false;

    _vecPos.clear();
 //   _vecCol.clear();
    // Setup positions and colors
    for (unsigned i = 0; i < newNb; ++i)
    {
        _vecPos.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
       // _vecCol.push_back(ofColor::fromHsb(255 * i / (float)newNb, 255, 255, 255));
    }

    _colorAmbi.updateVectorSize(newNb);

    _bDraw = true;
}
