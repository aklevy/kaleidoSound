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

/*#include "Network/Address.h"
#include "Network/Device.h"
#include "Network/Protocol/Local.h"
#include "Network/Protocol/Minuit.h"
*/
#include "ofxOssia.hpp"
#include "colorAmbience.hpp"
#include "MSAPhysics3D.h"

using namespace msa::physics;

class Shapes
{
public:
    Shapes() = delete; //forbids default constructor

    Shapes( World3D& world,ofEasyCam& cam,ossia::net::node_base& parentNode, std::string typeName, ofMesh mesh, int ambiType = 0);
    ~Shapes();

    void    setup();
    void    update();
    void    draw();
    void    reset();

    void    nbChanged(int &newNb);
    ofParameterGroup getParamGroup(){return _paramGroup;}

private:
    // Minuit parent Node
    ossia::net::node_base&          _parentNode;
    // Shape type name
    std::string                     _shapeType;

    // Parameter group to be exposed in ofApp's gui
    ofParameterGroup                _paramGroup;
    // Shapes number
    ossia::Parameter<int>           _nbShapes;

    // Shape type
    ofMesh                          _mesh;
    vector<Particle3D_ptr>          _particles;
    //vector<ofVec3f>                 _vecPos;

    // Color Ambience
    ColorAmbience                   _colorAmbi;
    ossia::Parameter<int>           _colorWarmCool;
    //vector<ofColor>     _vecCol;

    bool                _bDraw;
    //physics
    World3D& _world;
    ofEasyCam&                       _cam;


};

#endif
