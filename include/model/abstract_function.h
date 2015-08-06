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
 * File:   abstract_function.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 14:48
 */

#ifndef ABSTRACT_FUNCTION_H
#define	ABSTRACT_FUNCTION_H

#include "basic_includes.h"
#include "model/abstract_gradient.h"

/**
 * This pure virtual class provides an interface for the modeling functions f:R^2->R
 */
class abstract_function {
public:
    //Std constructor
    abstract_function() {
    };
    //Std destructor
    virtual ~abstract_function() {
    };

    /**
     * returns the value of the function at (x,y)
     * @param x coordinate
     * @param y coordinate
     * @return value of the function
     */
    virtual float eval(float x, float y) = 0;

    /**
     * Get the gradient function of this base function 
     * @return gradient function
     */
    virtual sharedptr<abstract_gradient> get_gradient() = 0;
    
};

#endif	/* ABSTRACT_FUNCTION_H */

