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
 * File:   strand_tile_unwrapper.h
 * Author: b.steltner
 *
 * Created on 10. November 2014, 14:07
 */

#ifndef STRAND_TILE_UNWRAPPER_H
#define	STRAND_TILE_UNWRAPPER_H

#include "algorithm/abstract_tile_unwrapper.h"

/**
 * @class Strand_tile_unwrapper
 * Unwrap single tile with the single block method from
 * Strand et al.: "Two-Dimensional Phase Unwrapping Using a Block Least-Squares Method", equations (4),(5) insbesondere
 */
class strand_tile_unwrapper : public abstract_tile_unwrapper {
public:
    /**
     * Constructor with explicit number of steps
     * @param N_rho Number of values in [0,2pi] to use for value rho  (equidistant)
     */
    strand_tile_unwrapper(int N_rho);

    /**
     * Constructor with vector of settings for the strand unwrapper
     * @param usettings 
     */
    strand_tile_unwrapper(std::vector<std::string> usettings);

    virtual ~strand_tile_unwrapper();

    void unwrap(boost::shared_ptr<tile> t);

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name();

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help();
private:
    //!Number of values in [0,2Pi] to use for value rho (equidistant)
    int N_rho;


    /**
     * Lokale Methode um die Norm aus dem Strand Paper zu berechnen, mit Abbruchbedingung
     * für Werte, die dann bereits größer sind als der aktuelle minimale Wert.
     * @param t boost::shared_ptr<tile>
     * @param cur_min_val Bei Methodenaufruf aktuelle "Minimum" von rho.
     * @return Norm 
     */
    float abs_diff_norm(boost::shared_ptr<tile> t, float cur_min_value);

};


#endif	/* STRAND_TILE_UNWRAPPER_H */

