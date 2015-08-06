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
 * File:   abstract_coordinate_system.h
 * Author: g.antonopoulos
 *
 * Created on 22. Dezember 2014, 16:15
 */

#ifndef ABSTRACT_COORDINATE_SYSTEM_H
#define	ABSTRACT_COORDINATE_SYSTEM_H

#include "tiles/tile.h"

/**
 * This is an abtstract class that calculates a coordinate (x,y) from an index
 * (iw,ih) for a tile. Any derived class need not be cartesian but could also
 * be polar.
 * 
 * USAGE:
 * Before the calc_... methods can be called, the coordinate system has to
 * be initialized with the given tile using the init(...) method. So the object
 * itself just lays the rules for how to calculate a coordinate (x,y) from
 * a given index (iw,ih). 
 */
class abstract_coordinate_system
{
private:

public:
    
    /**
     * Constructor. Before the coordinate system can be used, it has to be
     * initialized with the given tile. Only after the init(...) method is called
     * will the calculations functions give useful results.
     */
    abstract_coordinate_system() {}
    
    /**
     * This method initializes the coordinate system with the dimensions of the
     * given tile. This method must be called before the calc_... methods are
     * called.
     * @param t
     */
    virtual void init(sharedptr<tile> t) =0;
    
    // Destructor
    virtual ~abstract_coordinate_system() {}
        
    /**
     * This is a method that returns the distance between neighboring points at
     * point iw,ih. It was implemented to enable calculating the gradient in non 
     * equidistant coordinate systems (Jacobian).
     * @param iw
     * @param ih
     * @return deltax
     */
    virtual float calc_delta_x(long iw, long ih) =0; //todo reicht das für generelle grad bestimmung?
    
    //! See calc_delta_x_at
    virtual float calc_delta_y(long iw, long ih) =0;
    
    //! Calculate x coordinate at given pixel
    virtual float calc_x_at(long iw, long ih) =0;

    //! Calculate y coordinate at given pixel
    virtual float calc_y_at(long iw, long ih) =0;

};



#endif	/* ABSTRACT_COORDINATE_SYSTEM_H */

