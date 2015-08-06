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
 * File:   second_order_gradient.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 16. Dezember 2014, 11:04
 */

#ifndef SECOND_ORDER_GRADIENT_H
#define	SECOND_ORDER_GRADIENT_H

#include "algorithm/abstract_gradient_calculator.h"

//FORWARD DECLARATION START
class abstract_coordinate_system; 
//FORWARD DECLARATION END

/**
 * This class calculates the numerical derivative of a tile using a
 * cartesian_unit_coordinate system as the underlying coordinate system of each
 * tile.
 */
class second_order_gradient : public abstract_gradient_calculator {
private:
    sharedptr<abstract_coordinate_system> coordinate_system;
public:
    //!Std Constructor. This class always uses a cartesian_unit_coordinate system as the underlying coordinate system.
    second_order_gradient();

    //!Constructor  
    second_order_gradient(sharedptr<abstract_coordinate_system> coord_sys);

    //!Destructor
    virtual ~second_order_gradient();


    /**
     * Get the gradient of a given tile as raw float data
     * @param t tile
     * @return vector of floats, first holds the x-gradient-values in order of tile has in its vector. Second holds y...
     */
    sharedptr<std::vector<float> > get_gradient_floats(sharedptr<tile> t);
    
    /**
     * Get the gradient of a given tile as a Eigen::Vector of floats
     * @param t tile
     * @return Eigen::vector of floats, first holds the x-gradient-values in order of tile has in its vector. Second holds y...
     */
    sharedptr<Eigen::VectorXf> get_gradient_eigen_vector(sharedptr<tile> t);
};

#endif	/* GRADIENT_CALCULATOR_H */