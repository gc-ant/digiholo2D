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
 * File:   abstract_tile_unwrapper.h
 * Author: g.antonopoulos
 *
 * Created on 28. Oktober 2014, 08:42
 */

#ifndef ABSTRACT_TILE_UNWRAPPER_H
#define	ABSTRACT_TILE_UNWRAPPER_H

#include "basic_includes.h"
#include "tiles/tile.h"

/**
 * This pure virtual class provides an abstract interface for an unwrapper
 * that unwraps a single tile. It operates with boost smart pointers.
 */
class abstract_tile_unwrapper {
public:

    /**
     * Konstruktor
     */
    abstract_tile_unwrapper() {
    }

    virtual ~abstract_tile_unwrapper() {
    }

    /**
     * Unwrap a given tile.
     * @param t
     */
    virtual void unwrap(boost::shared_ptr<tile> t) = 0;

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name() = 0;

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help() = 0;
};


#endif	/* ABSTRACT_TILE_UNWRAPPER_H */

