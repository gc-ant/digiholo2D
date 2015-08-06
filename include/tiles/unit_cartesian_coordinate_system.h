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
 * File:   unit_coordinate_system.h
 * Author: g.antonopoulos
 *
 * Created on 22. Dezember 2014, 15:09
 */

#ifndef UNIT_COORDINATE_SYSTEM_H
#define	UNIT_COORDINATE_SYSTEM_H

#include "tiles/tile.h"
#include "tiles/abstract_coordinate_system.h"

/**
 * This is a class that represents the unit cartesian coordinate system of a tile.
 * For a given pixel coordinate iw,ih this class provides a method to obtain the
 * x coordinate (corresponding to the width-direction) and the y coordinate of the
 * pixel coordinate system.
 * The coordinate system for this class is defined as such:
 * Coordinates are given in (x,y)
 * 
 * (0,0)------------------>(1,0)
 * |                      |
 * |                      |
 * |        (x,y) (floating point values!)
 * |                      |
 * v                      v
 * (1,0)------------------>(1,1)
 *  * 
 * These coordinates will hold even if the tile is NOT QUADRATIC! This makes
 * it easy to define model functions that live on this unit coordinate system
 * indepedent of the shape and number of pixels of the tiles.
 * 
 * 
 */
class unit_cartesian_coordinate_system : public abstract_coordinate_system
{
private:
    
    //! Width and height of the tile
    long tile_width, tile_height;
    //! Space between two pixels in the unit coordinate system
    float delta_x, delta_y;
    
public:
    
    /**
     * Constructor. Before the coordinate system can be used, it has to be
     * initialized with the given tile. Only after the init(...) method is called
     * will the calculations functions give useful results.
     */
    unit_cartesian_coordinate_system();
    
    /**
     * This method initializes the coordinate system with the dimensions of the
     * given tile. This method must be called before the calc_... methods are
     * called.
     * @param t
     */
    virtual void init(sharedptr<tile> t);
    
    // Destructor
    virtual ~unit_cartesian_coordinate_system();
        
    /**
     * This is a method that returns the distance between neighboring points at
     * point iw,ih. It was implemented to enable calculating the gradient in non 
     * equidistant coordinate systems (Jacobian). Here it just returns a constant.
     * @param iw
     * @param ih
     * @return deltax
     */
    virtual float calc_delta_x(long iw, long ih);
    
    //! See calc_delta_x_at
    virtual float calc_delta_y(long iw, long ih);
    
    //! Special overloaded method that gives the equidistant space between points in x
    virtual float get_delta_x();
    
    //! See get_delta_x
    virtual float get_delta_y();

    
    
    //! Calculate x coordinate at given pixel
    float calc_x_at(long iw, long ih);

    //! Calculate y coordinate at given pixel
    float calc_y_at(long iw, long ih);

};


#endif	/* UNIT_COORDINATE_SYSTEM_H */

