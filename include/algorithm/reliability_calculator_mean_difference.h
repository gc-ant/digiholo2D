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
 * File:   reliability_calculator_mean_difference.h
 * Author: b.steltner
 *
 * Created on 25. November 2014, 11:16
 */

#ifndef RELIABILITY_CALCULATOR_MEAN_DIFFERENCE_H
#define	RELIABILITY_CALCULATOR_MEAN_DIFFERENCE_H

#include "algorithm/abstract_reliability_calculator.h"

class reliability_calculator_mean_difference : public abstract_reliability_calculator {
public:
    reliability_calculator_mean_difference();
    virtual ~reliability_calculator_mean_difference();
    /**
     * Calculates the reliability of a given junction
     * @param ti tiled image
     * @param t junction
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> t);

    /**
     * Calculates the reliability of a given tile
     * @param ti tiled image
     * @param t junction
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t);

    /**
     * Calculate the variance of each junction once, therefor making it possible to just access them via get_variance.
     * @param ti tiled image
     */
    void init_junctions(sharedptr<tiled_image> ti);
private:
    /**
     * A function is needed with maxima on 2nPi and minima at (2n+1)Pi  (n integer). Differences with 2nPi are "good"
     * values (because the tiles are matching modulo 2pi). Differences with (2n+1)Pi are bad, because they are have the 
     * worst possible offset against each other (shifting with multiples of 2pi is not changing the "badness" of the junction.
     * @param value Mean difference of a junction
     * @return 1 for best reliability, 0 for worst reliability
     */
    float reliability_function(float value);
};


#endif	/* RC_SECOND_DIFFERENCE_H */

