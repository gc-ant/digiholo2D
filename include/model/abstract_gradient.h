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
 * File:   abstract_gradient.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 14:52
 */

#ifndef ABSTRACT_GRADIENT_H
#define	ABSTRACT_GRADIENT_H

#include "basic_includes.h"

class abstract_gradient
{
public:

    abstract_gradient() {
    };

    virtual ~abstract_gradient() {
    };

    /**
     * returns the value of the function at (x,y)
     * @param x coordinate
     * @param y coordinate
     * @return value of the gradient function in x and y 
     */
    virtual sharedarray<float> eval(float x, float y) = 0;

};

#endif	/* ABSTRACT_GRADIENT_H */

