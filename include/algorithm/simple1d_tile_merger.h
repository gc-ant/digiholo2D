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
 * File:   simple1d_tile_merger.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 11. November 2014, 10:09
 */

#ifndef SIMPLE1D_TILE_MERGER_H
#define	SIMPLE1D_TILE_MERGER_H

#include "algorithm/abstract_tile_merger.h"
#include "tiles/tile.h"

class simple1d_tile_merger : public abstract_tile_merger {
public:

    /**
     * Linear unwrapper based on 1d unwrapping of blocks.
     */
    simple1d_tile_merger();

    /**
     * Linear unwrapper based on 1d unwrapping of blocks.The booleans are useful if the image has discontinuties on the edges.
     * @param left2right Specify the horizontal direction of merging: true: Start left and head right. false: right to left.
     * @param top2bottom Specify the vertical direction of merging: true: Start from the top to the bottom edge. false: bottom to top.
     * @param start_horizontal Specify order of merging: true: First merge horizontal, then vertical. false: First vertical, then horizontal
     */
    simple1d_tile_merger(bool left2right, bool top2bottom, bool start_horizontal);

    virtual ~simple1d_tile_merger();
    /**
     * Merge tiles of a tiled image. The tiles have to be unwrapped inside
     * themselves. This method unwraps the tiles with respect to each other. 
     * The phase discontinuity between each tile must be integer multiples of 2PI.
     * @param ti boost shared_ptr to smart tiled image
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
    bool left2right, top2bottom, start_horizontal;
    void tile_unwrap1d_horizontal(boost::shared_ptr<tiled_image> ti, bool left2right);
    void tile_unwrap1d_vertical(boost::shared_ptr<tiled_image> ti, bool top2bottom);
    float calculate(boost::shared_ptr<tiled_image> ti, long w, long h, tile::rel_position pos);
};

#endif	/* SIMPLE1D_TILE_MERGER_H */

