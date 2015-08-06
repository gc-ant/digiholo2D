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
 * File:   monomial_function.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 15:16
 */

#ifndef MONOMIAL_FUNCTION_H
#define	MONOMIAL_FUNCTION_H

#include "model/abstract_function.h"

//FORWARD DECLARATION START
class monomial_gradient; 
//FORWARD DECLARATION END

class monomial_function : public abstract_function {
public:
    /**
     * Constructor for x^m * y^n monomial
     * @param m power of x 
     * @param n power of y
     */
    monomial_function(int m, int n);

    //!Destruktor
    virtual ~monomial_function();

    /**
     * Calculate the value of this function
     * @param x coordinate
     * @param y coordinate
     * @return value of this function
     */
    float eval(float x, float y);

    /**
     * @return sharedptr to the gradient of this function
     */
    sharedptr<abstract_gradient> get_gradient();

    //!Experimental
    int get_x_exp();
    //!Experimental
    int get_y_exp();
private:
    //! Hidden std constructor
    monomial_function();

    //! x & y exponents 
    int x_exp, y_exp;
    //!Sharedptr to its gradient
    sharedptr<abstract_gradient> gradient;
};

#endif	/* MONOMIAL_FUNCTION_H */

