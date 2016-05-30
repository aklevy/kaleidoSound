#include "shapes.hpp"


Shapes::Shapes(std::shared_ptr<Node> parentNode, string typeName, ofMesh mesh)
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
        _vecCol.push_back(ofColor::fromHsb(255 * i / (float) _nbShapes, 255, 255, 255));
    }

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
            ofSetColor(_vecCol[i]);
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
    _vecCol.clear();
    // Setup positions and colors
    for (unsigned i = 0; i < newNb; ++i)
    {
        _vecPos.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
        _vecCol.push_back(ofColor::fromHsb(255 * i / (float)newNb, 255, 255, 255));
    }

    _bDraw = true;
}
