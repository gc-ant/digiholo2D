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
 * File:   srncp_tile_merger.h
 * Author: b.steltner
 *
 * Created on 25. November 2014, 11:48
 */

#ifndef SRNCP_TILE_MERGER_H
#define	SRNCP_TILE_MERGER_H


#include "algorithm/abstract_tile_merger.h"
#include <map>

//FORWARD DECLARATION START
class abstract_reliability_calculator;
//FORWARD DECLARATION END

class srncp_tile_merger : public abstract_tile_merger {
public:
    /**
     * Constructor used in the programm with settings as parameters
     * @param msettings settings for the merger
     */
    srncp_tile_merger(std::vector<std::string> msettings);
    
    /**
     * Explicit constructor with predefined reliability calculator
     * @param rc reliability calculator
     */
    srncp_tile_merger(boost::shared_ptr<abstract_reliability_calculator> rc);
    
    virtual ~srncp_tile_merger();
    /**
     * Merge all given tiles inside the  tiled image
     * @param ti boost shared_ptr to  tiled image
     */
    virtual void merge_tiles(boost::shared_ptr<tiled_image> ti);

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name();

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help();
private:
    /**
     * Hidden std constructor
     */
    srncp_tile_merger();
    
    /**
     * The reliability calculator which should be used in this instance 
     */
    boost::shared_ptr<abstract_reliability_calculator> rc;
    
    int series_start; 
    int series_stop;
    int series_steps;
    
    std::string name_extend; 
    
    /**
     * Calculate the reliability for all junctions and save it together with the ptr to the junction into a multimap. 
     * @param ti smart tiled image
     * @param junc_rel multimap in which the (float, shared_ptr<junction>) pair should be inserted in descending order
     */
    void initialise_merger(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<std::multimap<float, boost::shared_ptr<tile_junction>, std::greater<float> > > junc_rel);
    
    /**
     * Write the intermediate image to harddrive
     * @param ti the tiled image
     * @param name additional naming (identifying and prevent overwriting)
     */
    void write_intermediate(sharedptr<tiled_image> ti, std::string name);
};


#endif	/* SRNCP_TILE_MERGER_H */

