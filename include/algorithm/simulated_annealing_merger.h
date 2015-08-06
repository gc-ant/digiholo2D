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
 * File:   simulated_annealing_merger.h
 * Author: b.steltner
 *
 * Created on 6. Januar 2015, 14:19
 */

#ifndef SIMULATED_ANNEALING_MERGER_H
#define	SIMULATED_ANNEALING_MERGER_H

#include "algorithm/abstract_tile_merger.h"

class simulated_annealing_merger : public abstract_tile_merger {
public:
    /**
     * Default Constructor
     * @param msettings 
     */
    simulated_annealing_merger(std::vector<std::string> msettings);

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
     */
    simulated_annealing_merger(int check_steps, float convergence_criterion, std::string merge_prior, float name_energy_after_merging, float name_energy_prior_merging, bool name_extended, int name_steps_to_convergence, long seed, int series_start, int series_steps, int series_stop, float temperature);
    
    virtual ~simulated_annealing_merger();

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
    simulated_annealing_merger();
    
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

};

#endif	/* SIMULATED_ANNEALING_MERGER_H */

