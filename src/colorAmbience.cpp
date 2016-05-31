#include "colorAmbience.hpp"

ColorAmbience::ColorAmbience()
    :_ambienceType(0), //cool
      _sizeVec(0)
{

}
//---------------------------------------------------
void ColorAmbience::setup(int vectorSize, int ambienceType)
{
    if(_sizeVec != vectorSize)
    {
        _sizeVec = vectorSize;
    }

    changeAmbience(ambienceType);
}

//---------------------------------------------------
void ColorAmbience::changeAmbience(int& newAmbiType)
{
    if(_ambienceType != newAmbiType)
    {
         _ambienceType = newAmbiType;
        updateAmbience();
    }
}
//---------------------------------------------------
void ColorAmbience::updateAmbience()
{

    _colorVec.clear();


    if(_ambienceType <= 100)
        warmCoolAmbiance();

    else
        ofLog() << "ColorAmbience : Not registered ambience type = " << _ambienceType;



}
//---------------------------------------------------
void ColorAmbience::updateVectorSize(int& newVecSize)
{
    if(_sizeVec != newVecSize)
    {
        _sizeVec = newVecSize;
        updateAmbience();
    }


}
//---------------------------------------------------
void ColorAmbience::warmCoolAmbiance()
{

    if(_ambienceType<50) //cool
    {
        for (unsigned i = 0; i <  _sizeVec; ++i)
        {
            _colorVec.push_back(ofColor(200 * i / (float)  _sizeVec, // red
                                                 ofRandom(100,255), //green
                                                 ofRandom(200,255) + _ambienceType, // blue
                                                 255));
        }
    }
    else if(_ambienceType<80) //warm
    {
        for (unsigned i = 0; i <  _sizeVec; ++i)
        {
            _colorVec.push_back(ofColor( ofRandom(150 + _ambienceType ,255), // red
                                                 ofRandom(150 - _ambienceType,255) , //green
                                                 (_ambienceType/4) * i / (float)  _sizeVec, // blue
                                                 255));
        }
    } else
    {
        for (unsigned i = 0; i <  _sizeVec; ++i)
        {
            _colorVec.push_back(ofColor( ofRandom(220,255), // red
                                                 ofRandom(0,80) , //green
                                                 ofRandom(0,10), // blue
                                                 255));
        }
    }

}
//---------------------------------------------------
ofColor ColorAmbience::getColor(int index)
{
    if(!_colorVec.empty() && index < _colorVec.size() && index >= 0)
        return _colorVec[index];
    return ofColor(255,255,255,255);
}
