/*
* Copyright 2015 G.C. Antonopoulos and B. Steltner, Laser Zentrum Hannover e.V.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* 
 * File:   measure_pixel_energy.h
 * Author: b.steltner
 *
 * Created on 23. Januar 2015, 10:57
 */

#ifndef MEASURE_PIXEL_ENERGY_H
#define	MEASURE_PIXEL_ENERGY_H

#include "basic_includes.h"

//FORWARD DECLARATION START
class row_major_float_image; 
//FORWARD DECLARATION END


/**
 * This class provides a measure of the quality for a image provided.
 * The quality is set to the summation over every neighbouring tiles squared difference.
 * Therefore faulty unwraps result in a big penalty, increasing the "energy" of the image.
 * Note: This measure is not definite, it only provides a option to compare two familiar images.
 * E.g. compare a image unwrapped with unwrapper#1 and the same image unwarpped with unwrapper#2.
 */
class measure_pixel_energy  {
public:
    //!Std constructor
    measure_pixel_energy();
    //!Destructor
    virtual ~measure_pixel_energy();
    
    /**
     * This method calculates the energy of the image
     * @param image the image
     * @return energy-value
     */
    float calc(sharedptr<row_major_float_image> image);
};


#endif	/* MEASURE_PIXEL_ENERGY_H */

