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
 * File:   abstract_gradient_calculator.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 16. Dezember 2014, 11:14
 */

#ifndef ABSTRACT_GRADIENT_CALCULATOR_H
#define	ABSTRACT_GRADIENT_CALCULATOR_H

#include "basic_includes.h"
#include "Eigen/Dense"

//FORWARD DECLARATION START
class unit_cartesian_coordinate_system;
class tile; 
//FORWARD DECLARATION END

class abstract_gradient_calculator {
public:

    /**
     * This constructor takes a pointer to an abstract_coordinate system. This
     * coordinate system is then used to calculate the derivative for the given tile
     * in the get_gradient_... methods.
     * @param coord_sys
     */
    abstract_gradient_calculator() {
    };
    //!Destructor

    virtual ~abstract_gradient_calculator() {
    };

    /**
     * Get the gradient of a given tile in the tile-format. First element of the
     * vector will be a tile representing the x gradient and second element
     * will be a tile representing the y gradient. This method is for debug purposes
     * mostly since it will call the abstract get_gradient_floats method of the
     * instance.
     * @param t tile
     * @return gradient of given tile in x and y direction. x is first, y is second
     */
    virtual sharedptr<std::vector<tile> > get_gradient_tile(sharedptr<tile> t);

    /**
     * Get the gradient of a given tile as raw float data.
     * @param t tile
     * @return vector of floats, first holding the x-gradient-values in same order which the data array of the tiles stores the values. Afterwards appending the y-values...
     */
    virtual sharedptr<std::vector<float> > get_gradient_floats(sharedptr<tile> t) = 0;

    /**
     * Get the gradient of a given tile as a Eigen::Vector of floats
     * @param t tile
     * @return Eigen::vector of floats, first holds the x-gradient-values in order of tile has in its vector. Second holds y...
     */
    virtual sharedptr<Eigen::VectorXf> get_gradient_eigen_vector(sharedptr<tile> t) = 0;

};

#endif	/* ABSTRACT_GRADIENT_CALCULATOR_H */

