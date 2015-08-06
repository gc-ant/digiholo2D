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
 * File:   grad_fit_tile_unwrapper.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 5. Dezember 2014, 11:53
 */

#ifndef SMART_GRAD_FIT_TILE_UNWRAPPER_H
#define	SMART_GRAD_FIT_TILE_UNWRAPPER_H

#include "algorithm/abstract_tile_unwrapper.h"

class grad_fit_tile_unwrapper : public abstract_tile_unwrapper {
public:
    //    grad_fit_tile_unwrapper();
    //    ~grad_fit_tile_unwrapper();

    void unwrap(sharedptr<tile> t);

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name();

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help();
};


#endif	/* SMART_GRAD_FIT_TILE_UNWRAPPER_H */

