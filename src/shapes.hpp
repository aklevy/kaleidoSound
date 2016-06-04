#ifndef SHAPES_H
#define SHAPES_H


#if defined(Bool)
#undef Bool
#endif
#if defined(True)
#undef True
#endif
#if defined(False)
#undef False
#endif

#include "Network/Address.h"
#include "Network/Device.h"
#include "Network/Protocol/Local.h"
#include "Network/Protocol/Minuit.h"

#include "parameter.hpp"
#include "colorAmbience.hpp"
#include "MSAPhysics3D.h"

using namespace msa::physics;

class Shapes
{
public:
    Shapes() = delete; //forbids default constructor

    Shapes( World3D& world,std::shared_ptr<Node> parentNode, std::string typeName, ofMesh mesh, int ambiType = 0);
    ~Shapes();

    void    setup();
    void    update();
    void    draw();
    void    reset();

    void    nbChanged(int &newNb);

    ofParameterGroup getParamGroup(){return _paramGroup;}

private:
    // Minuit parent Node
    std::shared_ptr<Node>           _parentNode;

    // Shape type name
    std::string                     _shapeType;

    // Parameter group to be exposed in ofApp's gui
    ofParameterGroup                _paramGroup;
    // Shapes number
    Parameter<int>                  _nbShapes;

    // Shape type
    ofMesh                          _mesh;
    vector<Particle3D_ptr>          _particles;
    //vector<ofVec3f>                 _vecPos;

    // Color Ambience
    ColorAmbience                   _colorAmbi;
    Parameter<int>                  _colorWarmCool;
    //vector<ofColor>     _vecCol;

    bool                _bDraw;

    //physics
    World3D& _world;
};

#endif
