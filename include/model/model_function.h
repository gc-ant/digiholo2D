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
 * File:   model_function.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 15:07
 */

#ifndef MODEL_FUNCTION_H
#define	MODEL_FUNCTION_H

#include "model/abstract_function.h"
#include "Eigen/Dense"

//FORWARD DECLARATION START
class monomial_function; 
class abstract_coordinate_system; 
class tile; 
//FORWARD DECLARATION END

/**
 * This class implements a vector of base functions. These are linear combined to a full function, providing a method to model the image
 * with functions. Major advantage is that the functions itself must not be linear, nor be constants. Polynomial functions as well as a linear
 * combination of exponentiation of sin/cosin.
 * !! Base functions must not be constants. A constant offset is treated separately by the model function. It is set to 0 on initialization.
 */
class model_function : public abstract_function {
protected:
    //! The model functions holds the coordinate system used.
    sharedptr<abstract_coordinate_system> coordinate_system;
   
private:
    //!This is a constant that is added to the value of the linear combination of base functions. Base functions must not be constants.
    float const_offset;

public:
    //!Std constructor
    model_function();

    /**
     * @param vc_base vector of base functions this method should use
     * @param coord_system coordinate system in use with this base functions
     */
    model_function(sharedptr<std::vector<sharedptr<abstract_function> > > vc_base, sharedptr<abstract_coordinate_system> coord_system);

    //!Destruktor
    virtual ~model_function();

    /**
     * Evaluates the value of this function with the coefficients this model itself holds
     * @param x coordinate
     * @param y coordinate
     * @return value of this function
     */
    float eval(float x, float y);

    /**
     * Evaluates the value of this function with the coefficients provided
     * @param x coordinate
     * @param y coordinate
     * @param vc_coeff set of coeff to scale the base functions. NOTE: Must have the same size as the base functions...
     * @return value of this function
     */
    float eval(float x, float y, sharedptr<std::vector<float> > vc_coeff);

    /** Evaluates this function with the coefficients provided, see overloaded method*/
    float eval(float x, float y, sharedptr<Eigen::VectorXf> v_coeff);

    /**   
     * SERVES NO PURPOSE. 
     * Implementation is purly because its a inherited pure virtual function by abstract function class
     * @return gradient NOT MEANINGFULL - NOT EVEN INITIALISED
     */
    sharedptr<abstract_gradient> get_gradient();
    
    //! Returs a pointer to the used coordinate system. This is the unit cartesian system given by the second_order_gradient calculator.
    sharedptr<abstract_coordinate_system> get_coordinate_system();

    /**
     * Set (that is replace) the current set of coefficients 
     * @param coeff set of coefficients
     */
    void set_coeff_vector(sharedptr<std::vector<float> > coeff);

    //! Overloaded function to work with Eigen::VectorXf
    void set_coeff_vector(sharedptr<Eigen::VectorXf> coeff);

    /**
     * Add (or replace!) the set of base functions
     * @param vc_base
     */
    void set_base_vector(sharedptr<std::vector<sharedptr<abstract_function> > > vc_base);

    //!Set the constant offset of this function to the specified value.
    void set_constant_offset(float value);

    /**
     * @return The set of base functions
     */
    sharedptr<std::vector<sharedptr<abstract_function> > > get_base_vector();

    /**
     * Get the size of the coefficients set
     * @return size of the vc_coeff
     */
    int get_coeff_size();

    /**
     * Get the size of the base set
     * @return size of the vc_base_functions
     */
    int get_base_size();

    //!Return the constant offset of this function.
    float get_constant_offset();
    
    /**
     * This method subtracts a function from the pixels of the tile. The
     * underlying coordinate system on which the function is evaluated for
     * every pixel is given as the second argument. There is no 
     * check, that the given function is defined on the given coordinate system.
     * So if the function expects polar coordinates but a cartesian system is
     * used the evaluation of the function will return values that are not useful.
     * @param tile
     * @param number of steps for the strand unwrapper to make
     */
    void subtract_wrap_add_function(sharedptr<tile> t, int step_count);

private:

    //! Vector of coefficients belonging to the base functions
    sharedptr<std::vector<float> > vc_coeff;
    //! Vector of base functions
    sharedptr<std::vector<sharedptr<abstract_function> > > vc_base_functions;
    

    //! SERVES NO PURPOSE. NOT MEANINGFUL, NOT EVEN INITIALISED
    sharedptr<abstract_gradient> gradient;
};

#endif	/* MODEL_FUNCTION_H */

