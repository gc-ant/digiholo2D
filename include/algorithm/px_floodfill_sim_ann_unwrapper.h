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
 * File:   px_floodfill_sim_ann_unwrapper.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 17. März 2015, 11:16
 */

#ifndef PX_FLOODFILL_SIM_ANN_UNWRAPPER_H
#define	PX_FLOODFILL_SIM_ANN_UNWRAPPER_H

#include "basic_includes.h"
#include "image/float_image.h"


/**
 * Pixelbased Floodfill Simulated Annealing Unwrapper
 */
class px_floodfill_sim_ann_unwrapper : abstract_unwrapper
{
private:
    //
public:
        
    /**
     * 
     */
    px_floodfill_sim_ann_unwrapper();
    
    //!Destructor
    virtual ~px_floodfill_sim_ann_unwrapper();
    
    //!Unwrap the phase image
    virtual boost::shared_ptr<float_image> unwrap(boost::shared_ptr<float_image> wrapped_phase_image);
    
    /**
     * Method to floodfill all pixels that are connected (without phase jumps) to a pixel at a given position.
     * 
     */
    void floodfill(boost::shared_ptr<float_image>, int iw, int ih)
    
};

#endif	/* PX_FLOODFILL_SIM_ANN_UNWRAPPER_H */

