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
 * File:   reliability_calculator_random.h
 * Author: b.steltner
 *
 * Created on 22. Januar 2015, 09:13
 */

#ifndef RELIABILITY_CALCULATOR_RANDOM_H
#define	RELIABILITY_CALCULATOR_RANDOM_H

#include "algorithm/abstract_reliability_calculator.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>

/**
 * The methods of this class are just returning uniformly distributed random values between 0 and 1. 
 * This makes up a comparison to the other rel.-calculator regarding speed and output quality. 
 */
class reliability_calculator_random : public abstract_reliability_calculator {
public:
    reliability_calculator_random();

    virtual ~reliability_calculator_random();

    /**
     * Calculates the reliability of a given junction
     * @param ti tiled image
     * @param junc junction
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> junc);

    /**
     * Calculates the reliability of a given tile
     * @param ti tiled image
     * @param t tile
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t);

    /**
     * EMPTY
     * @param ti tiled image
     */
    void init_junctions(sharedptr<tiled_image> ti);
private:
    boost::random::mt19937 gen;
    boost::random::uniform_01<> dist;
};

#endif	/* RELIABILITY_CALCULATOR_RANDOM_H */

