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
 * File:   simulated_annealing_floodfill_merger.h
 * Author: b.steltner
 *
 * Created on 30. Januar 2015, 14:19
 */

#ifndef SIMULATED_ANNEALING_FLOODFILL_MERGER_H
#define	SIMULATED_ANNEALING_FLOODFILL_MERGER_H

#include "algorithm/abstract_tile_merger.h"
#include <unordered_set>
#include "algorithm/abstract_reliability_calculator.h"

class simulated_annealing_floodfill_merger : public abstract_tile_merger {
public:
    /**
     * Default Constructor
     * @param msettings 
     */
    simulated_annealing_floodfill_merger(std::vector<std::string> msettings);

    /**
     * Explicit constructor for this merger. 
     * Note: Options that should not be used are marked as: 
     * - std::strings should be *.empty() == true
     * - int should be set to -1
     * eg.: No merging preprocessing: mergie_prior = ""; No image series: series_steps = -1; 
     * @param check_steps Distance in steps when the abort parameter is calcualted again. Runtime w/o convergence is allthough dependant on this paramter.
     * @param convergence_criterion Criterion for the convergence - If after check_steps steps the detla_energy is higher than -convergence_criterion, convergence is assumed
     * @param merge_prior Name of the merger which should run prior the simann merger. If no pre-processing should be done, the string is empty
     * @param name_energy_after_merging naming variable for the get_name method
     * @param name_energy_prior_merging naming variable for the get_name method
     * @param name_extended naming variable for the get_name method - true if the output file name should contain important data from this merging process
     * @param name_steps_to_convergence naming variable for the get_name method
     * @param seed Seed for the random number generator. If seed and input_image are equal, the output will be the same!
     * @param series_start Image series paramater: At which step should the saving of the images start.
     * @param series_steps Every series_steps step a image should be saved
     * @param series_stop Image series parameter: At which step should the saving of the image stop.
     * @param temperature Changes the accept guess probability. Highter temperatures makes more "bad" jumps possible
     * @param good_mean What value of the junction->mean is acceptable for the floodfill to step on to the next tile
     * @param good_variance Up to which value is the current tile allowed to spread the floodfill. With a lower value the floodfill will not spread from this tile
     */
    simulated_annealing_floodfill_merger(int check_steps, float convergence_criterion, std::string merge_prior, float name_energy_after_merging, float name_energy_prior_merging, bool name_extended, int name_steps_to_convergence, long seed, int series_start, int series_steps, int series_stop, float temperature, float good_mean, float good_variance);

    virtual ~simulated_annealing_floodfill_merger();

    /**
     * Merge all given tiles inside the tiled image
     */
    virtual void merge_tiles(sharedptr<tiled_image> ti);

    /**
     * Return a name (with options) to set in the output file name
     */
    virtual std::string get_name();

    /**
     * Display a help on how to use this merger, in particular usage of merger-settings
     */
    virtual void usage_help();
private:
    //!Hidden std constructor
    simulated_annealing_floodfill_merger();

    //! Distance in steps when the abort parameter is calcualted again. Runtime w/o convergence is allthough dependant on this paramter.
    int check_steps;
    //! Criterion for the convergence - If after check_steps steps the detla_energy is higher than -convergence_criterion, convergence is assumed
    float convergence_criterion;
    //! Temperature - Changes the accept guess probability. Highter temperatures makes more "bad" jumps possible
    float temperature;
    //! Name of the merger which should run prior the simann merger. If no pre-processing should be done, the string is empty
    std::string merge_prior;
    //! naming variable for the get_name method
    int name_steps_to_convergence;
    //! naming variable for the get_name method - true if the output file name should contain important data from this merging process
    bool name_extended;
    //! naming variable for the get_name method
    float name_energy_prior_merging;
    //! naming variable for the get_name method
    float name_energy_after_merging;

    //! Image series paramater: At which step should the saving of the images start.
    int series_start;
    //! Image series parameter: At which step should the saving of the image stop.
    int series_stop;
    //! Image series paramter: Every series_steps step a image should be saved
    int series_steps;

    //! Seed for the random number generator. If seed and input_image are equal, the output will be the same!
    long seed;
    
    //! What value of the junction->mean is acceptable for the floodfill to step on to the next tile
    float good_mean; 
    //! Up to which value is the current tile allowed to spread the floodfill. With a lower value the floodfill will not spread from this tile
    float good_variance; 

    
    
    /**
     * Adds +-2Pi to the tilegroups and checks wether the energy has decreased (Monte-Carlo-Method). 
     * @param ti tiled image 
     * @param current tile
     * @param prop_add probabilty to add value or subtract value
     * @param prop_accept probabilty of Monte-Carlo-Accept or Decline
     * @param tg_open Tilegroup of tiles with good reliability
     * @param tg_closed Tilegroup of tiles with bad reliability
     * @param rc
     * @param junc_arr_size number of junctions
     * @param step number of steps this method has been called already
     */
    void add_value_and_check(sharedptr<tiled_image> ti, sharedptr<tile> cur_tile,  float prop_add, float prop_accept, sharedptr<tilegroup> tg_open, sharedptr<tilegroup> tg_closed, sharedptr<abstract_reliability_calculator> rc, long junc_arr_size, long step); 
    
    /**
     * Fill a tilegroup with every neighbour (and its neighbours neighbours etc.) that have no 
     * phase jumps between them and have no inconsistencies in the tile itself.
     * @param ti tiled image
     * @param t current tile
     * @param tg_open will be filled with every allowed tile
     * @param tg_closed will be filled with every tile that may be allowed, but has inconsistencies in the tile itself and is thus disallowed of spreading.
     * @param rc reliability calculator
     */
    void floodfill(sharedptr<tiled_image> ti, sharedptr<tile> t, sharedptr<tilegroup> tg_open, sharedptr<tilegroup> tg_closed, sharedptr<abstract_reliability_calculator> rc);
    /**
     * Helper function for the floodfill method
     * @param ti tiled image
     * @param t current tile
     * @param neighbour_tile neighbour tile
     * @param tg_open will be filled with every allowed tile
     * @param tg_closed will be filled with every tile that may be allowed, but has inconsistencies in the tile itself and is thus disallowed of spreading.
     * @param rc reliability calculator
     */
    void recur_floodfile(sharedptr<tiled_image> ti, sharedptr<tile> t, sharedptr<tile> neighbour_tile, sharedptr<tilegroup> tg_open, sharedptr<tilegroup> tg_closed, sharedptr<abstract_reliability_calculator> rc);

    /**
     * Calculate the energy of the given junctions. These should be the neighbouring junctions of a given tile, 
     * therefore this is the energy of the tile.
     * @param junc_vc vector of junctions
     * @return energy of the tile
     */
    float calc_energy(sharedptr<std::vector<sharedptr<tile_junction> > > junc_vc);

    /**
     * Write the intermediate image to harddrive
     * @param ti the tiled image
     * @param name additional naming (identifying and prevent overwriting)
     */
    void write_intermediate(sharedptr<tiled_image> ti, std::string name);

    /**
     * Print out all the options used in this merger...
     */
    void show_options();
};

#endif	/* SIMULATED_ANNEALING_FLOODFILL_MERGER_H */

