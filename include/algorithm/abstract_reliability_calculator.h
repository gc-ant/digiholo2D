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
 * File:   abstract_reliability_calculator.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 25. November 2014, 11:03
 */

#ifndef ABSTRACT_RELIABILITY_CALCULATOR_H
#define	ABSTRACT_RELIABILITY_CALCULATOR_H

#include "tiles/tiled_image.h"

/**
 * This pure virtual class provides an abstract interface for a reliability 
 * calculating class. Such a class must implement a function for calculating the
 * reliability of a given junction and of a given tile.
 */
class abstract_reliability_calculator {
public:

    /**
     * Constructor
     */
    abstract_reliability_calculator() {
    };

    virtual ~abstract_reliability_calculator() {
    };
    /**
     * Calculates the reliability of a given junction
     * @param ti tiled image
     * @param junc junction
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> junc) = 0;

    /**
     * Calculates the reliability of a given tile
     * @param ti tiled image
     * @param t tile
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t) = 0;

    /**
     * Calculate the variance of each junction once, therefor making it possible to just access them via get_variance.
     * @param ti tiled image
     */
    virtual void init_junctions(sharedptr<tiled_image> ti) = 0;
};

#endif	/* ABSTRACT_RELIABILITY_CALCULATOR_H */

