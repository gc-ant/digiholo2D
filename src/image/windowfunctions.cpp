#include "basic_includes.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "fringe/windowfunctions.h"


const float TWOPI = 2*M_PI;

float hann_window(float r)
{
    return 0.5*(1+cos(r*M_PI));
}

float pow_hann_window3(float r)
{
    return 0.5*(1+cos(r*M_PI))* 0.5*(1+cos(r*M_PI)) * 0.5*(1+cos(r*M_PI));
}

//http://de.wikipedia.org/wiki/Fensterfunktion
//Muss aber so abgewandelt werden, dass sie das Argument 0-1 akzeptiert und sinnvoll
//verarbeitet.
//ACHTUNG: DIES IST NICHT WIRKLICH EIN BLACKMAN-NUTTAL WINDOW sondern
//dem 1-blackman-nuttal
float blackman_nuttal_window(float r)
{
    const float a0 = 0.35875f;
    const float a1 = 0.48829;
    const float a2 = 0.14128;
    const float a3 = 0.01168;
    
    return 1.f-(a0 -a1*cos(M_PI*r)+a2*cos(2*M_PI*r)-a3*cos(3*M_PI*r));
    //return a0-a1*cos(M_PI*(1.f-r))+a2*cos(2*M_PI*(1.f-r))-a3*cos(3*M_PI*(1.f-r));
}