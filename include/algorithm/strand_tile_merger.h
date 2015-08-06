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
 * File:   strand_tile_merger.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 12. November 2014, 14:57
 */

#ifndef STRAND_TILE_MERGER_H
#define	STRAND_TILE_MERGER_H

#include "algorithm/abstract_tile_merger.h"

class strand_tile_merger : public abstract_tile_merger {
    friend void add_tile_to_group(sharedptr<tile> t, sharedptr<tilegroup> g);
public:
    /**
     * Konstruktor for a merger class, based on the (direct) merge-algorithm from the strand paper. 
     */
    strand_tile_merger();
    ~strand_tile_merger();

    /**
     * Merge all given tiles inside the smart tiled image.
     * @param ti boost shared_ptr to smart tiled image
     */
    void merge_tiles(sharedptr<tiled_image> ti);

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
     * Local help function. This Method calculates the "r" as in the paper
     * @param vec_junction_ptr vector of junctions
     * @return r as in the paper
     */
    float abs_diff_norm(sharedptr<std::vector<sharedptr<tile_junction> > > vec_junction_ptr);

};


#endif	/* STRAND_TILE_MERGER_H */

