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
 * File:   monomial_gradient.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 15:58
 */

#ifndef MONOMIAL_GRADIENT_H
#define	MONOMIAL_GRADIENT_H

#include "model/abstract_gradient.h"

class monomial_gradient : public abstract_gradient {
public:
    /**
     * Standard constructor. Takes the exponents m and n from the parent polynom.
     * Eg. x^3y^4 calls this gradient with m = 3 & n = 4. Conversion into the derivative 
     * comes in this constructor.
     * @param m power of x
     * @param n power of y
     */
    monomial_gradient(int m, int n);

    virtual ~monomial_gradient();

    /**
     * Calculate the gradient for this function
     * @param x coordinate
     * @param y coordinate
     * @return 2 floats, first for x-value 
     */
    virtual sharedarray<float> eval(float x, float y);
private:
    //!Hidden std. Constructor
    monomial_gradient();
    //!x & y exponents
    int x_exp, y_exp;
};


#endif	/* MONOMIAL_GRADIENT_H */

