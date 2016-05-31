#ifndef COLORAMBIENCE_H
#define COLORAMBIENCE_H

#include "ofMain.h"


class ColorAmbience
{
public:
    ColorAmbience();

    // setup the vector with the corresponding size and set ambience type
    void setup(int vectorSize, int ambienceType);

    // change ambience corresponding to the string in entry
    void changeAmbience(int &ambienceType);

    void updateAmbience();
    void updateVectorSize(int& newVecSize);

    ofColor getColor(int index);//{if(!_colorVec.empty() && index < _colorVec.size())  return _colorVec[index];}
private:

    void warmCoolAmbiance();

    // name of the ambience type (e.g. warm, cool ... )
    int             _ambienceType;

    // size of the vector of Color
    int                     _sizeVec;

    // Vector of Color, each corresponding to one shape's color
    std::vector<ofColor>    _colorVec;
/*

    ofColor                 _colorMin;
    ofColor                 _colorMax;*/
};

#endif
