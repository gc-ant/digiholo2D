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
 * File:   reliability_calculator_variance_second.h
 * Author: b.steltner
 *
 * Created on 4. Februar 2015, 11:32
 */

#ifndef RELIABILITY_CALCULATOR_VARIANCE_SECOND_H
#define	RELIABILITY_CALCULATOR_VARIANCE_SECOND_H

#include "abstract_reliability_calculator.h"

//FORWARD DECLARATION START
class tiled_image;
//FORWARD DECLARATION END

class reliability_calculator_variance_second : public abstract_reliability_calculator {
public:
    reliability_calculator_variance_second();


    virtual ~reliability_calculator_variance_second();

    /**
     * Calculates the reliability of a given junction
     * @param ti tiled image
     * @param t junction
     * @return float between 0 and FLT_MAX/2
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> t);

    /**
     * Calculates the reliability of a given tile
     * @param ti tiled image
     * @param t junction
     * @return float between 0 and FLT_MAX
     */
    virtual float calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t);
    void init_junctions(sharedptr<tiled_image> ti);
};

#endif	/* RELIABILITY_CALCULATOR_VARIANCE_SECOND_H */

