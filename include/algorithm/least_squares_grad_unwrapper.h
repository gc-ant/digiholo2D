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
 * File:   least_squares_grad_unwrapper.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 16. Dezember 2014, 11:29
 */

#ifndef LEAST_SQUARES_GRAD_UNWRAPPER_H
#define	LEAST_SQUARES_GRAD_UNWRAPPER_H

#include "algorithm/abstract_tile_unwrapper.h"
#include "Eigen/Dense"


//FORWARD DECLARATION START
class abstract_gradient_calculator; 
class second_order_gradient; 
class model_function; 
class monomial_function; 
//FORWARD DECLARATION END

/**
 * This class uses a general linear least squares approach for unwrapping the tiles (MLSQU).
 * General: Within its constructor, this class takes a given set of base functions, 
 * calculates the coefficients and fit them to the data. 
 * Linear: The choice of the base functions is up to the user. Only limitation is that 
 * these base functions have to be linear in the coefficients. Eg. a*x^5*y^3, b*x^2, c*1 is as good 
 * as a*sin(x), b*cos(x/4), c*tan(3). NOT working is sin(a*x), x^b, x^y. 
 * Least Squares: This approach leads to a big matrix / system of equastions, which will be 
 * solved with least squares methods, namely the SV-Decomposition.
 * 
 * This class will always use the second order gradient calculator for calculation
 * of the gradient profile of the tile. Thus it will always operate on a
 * unit_cartesian_coordinate_system
 * 
 */
class least_squares_grad_unwrapper : public abstract_tile_unwrapper {
public:
    /**
     * This constructor is used in the main programm working with the command line option.
     * @param usettings vector of strings with options for the unwrapper
     */
    least_squares_grad_unwrapper(std::vector<std::string> usettings);

    /**
     * Constructor with explicit model function as a parameter
     */
    least_squares_grad_unwrapper(sharedptr<model_function> model);

    //!Destructor
    virtual ~least_squares_grad_unwrapper();

    /**
     * Unwrap a given tile.
     * @param t
     */
    void unwrap(sharedptr<tile> t);

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name();

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help();
private:
    //! Hidden std constructor
    least_squares_grad_unwrapper();

    //! Pointer to the gradient calculator object that is used.
    sharedptr<abstract_gradient_calculator> grad_calculator;

    //!todo doc
    sharedptr<Eigen::MatrixXf> A;

    //!Vector of base functions
    sharedptr<model_function> model;
    //!Vector of coefficients
    sharedptr<std::vector<float> > vc_coeff;
    //!Name of the options
    std::string options_name;
    
    int strand_steps; 


    /**
     * This is the singular value decomposition of the matrix A. It is stored
     * here because for tiles of the same dimensions the matrix A will not change
     * and hence the singular value decomposition will not either. The singular
     * value decomposition is needed to solve the least squares problem of fitting
     * a model function to the gradient of the tile.
     */
    sharedptr<Eigen::JacobiSVD<Eigen::MatrixXf> > svd_of_A;

    /** 
     * Calculates the matrix A and its SVD and stores it in the member variables
     * of this class. This method is called by the unwrap method if needed.
     * @param t tile
     * @return Design matrix of the fitting problem.
     */
    void calc_A_and_SVD(boost::shared_ptr<tile> t);

    /**
     * @param t tile
     * @return Vector b, holding 2*W*H float values. First "W*H" x-gradient and 2nd W*H y-gradient
     */
    sharedptr<Eigen::VectorXf> calc_and_get_b(sharedptr<tile> t);

    /**
     * Search trouhgh the usettings parameter and apply settings to this merger
     * @param usettings vector of settings for this unwrapper
     */
    void init(std::vector<std::string> usettings);

};
#endif	/* LEAST_SQUARES_GRAD_UNWRAPPER_H */

