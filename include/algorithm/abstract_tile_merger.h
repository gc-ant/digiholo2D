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
 * File:   abstract_tile_merger.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 11. November 2014, 10:13
 */

#ifndef ABSTRACT_TILE_MERGER_H
#define	ABSTRACT_TILE_MERGER_H

#include "tiles/tiled_image.h"

/**
 * This pure virtual class provides an abstract interface for an merger
 * that merges all tiles of a smart tiled image. It operates with boost smart pointers.
 */
class abstract_tile_merger {
public:

    /**
     * Konstruktor
     */
    abstract_tile_merger() {
    }

    virtual ~abstract_tile_merger() {
    }

    /**
     * Merge all given tiles inside the smart tiled image
     * @param t boost shared_ptr to smart tiled image
     */
    virtual void merge_tiles(boost::shared_ptr<tiled_image> t) = 0;

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name() = 0;

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help() = 0;
};

#endif	/* ABSTRACT_TILE_MERGER_H */

