#include "algorithm/wrap.h"
#include "basic_includes.h" //for M_PI

float wrapToPi(float angle)
{
    angle = fmod(angle + M_PI,2.f*M_PI);
    if (angle < 0)
    {
        angle += 2.f*M_PI;
    }
    return angle - M_PI;
}


//MUCH SLOWER
//float wrapToPi(float angle)
//{
//    return atan2(sin(angle), cos(angle));
//}