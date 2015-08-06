#include "algorithm/simulated_annealing_merger.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_01.hpp>

#include "debug/debug_time.h"
#include "debug/debug_file_io.h"

#include "algorithm/reliability_calculator_variance.h"
#include "algorithm/srncp_tile_merger.h"
#include "algorithm/simple1d_tile_merger.h"
#include "tiles/tile_junction.h"
#include "image/in_out.h"

simulated_annealing_merger::simulated_annealing_merger(int check_steps, float convergence_criterion, std::string merge_prior, float name_energy_after_merging, float name_energy_prior_merging, bool name_extended, int name_steps_to_convergence, long seed, int series_start, int series_steps, int series_stop, float temperature) :
    abstract_tile_merger(), check_steps(check_steps), convergence_criterion(convergence_criterion), temperature(temperature), merge_prior(merge_prior), name_steps_to_convergence(name_steps_to_convergence), name_extended(name_extended), name_energy_prior_merging(name_energy_prior_merging), name_energy_after_merging(name_energy_after_merging),  series_start(series_start), series_stop(series_stop), series_steps(series_steps), seed(seed)   {
    //empty
}

simulated_annealing_merger::simulated_annealing_merger(std::vector<std::string> msettings) {
    this->series_start = -1;
    this->series_stop = -1;
    this->series_steps = -1;
    this->temperature = -1;
    this->convergence_criterion = -1;
    this->check_steps = -1;
    this->name_extended = false;
    std::string option, value;

    std::vector<std::string>::iterator it;
    for (it = msettings.begin(); it != msettings.end(); ++it) {
        option = it->substr(0, it->find_first_of('-'));
        value = it->substr(it->find_first_of('-') + 1);
        if (option.compare("temp") == 0) {
            this->temperature = (!value.empty() ? boost::lexical_cast<float>(value) : 0.f);
            DEBUG_PRINTLN("temperature set to: " << this->temperature);
        } else if (option.compare("conv") == 0) {
            this->convergence_criterion = (!value.empty() ? boost::lexical_cast<float>(value) : 0.f);
            DEBUG_PRINTLN("convergence set to: " << this->convergence_criterion);
        } else if (option.compare("pre") == 0) {
            if (!value.empty()) this->merge_prior = value;
            else DEBUG_PRINTLN("Specify value simple/srncp for option pre- ...");
        } else if (option.compare("steps") == 0) {
            this->check_steps = (!value.empty() ? boost::lexical_cast<float>(value) : 0.f);
        } else if (option.compare("saveinfo") == 0) {
            this->name_extended = true;
        } else if (option.compare("series") == 0) {
            DEBUG_PRINTLN("Series-option was selected. DEFAULT printout is to C:\\ImageSeries\\SimAnn\\ . Please make sure this directory exists...")
            if (!value.empty()) {
                //value should look like: x or x-y-z
                //                DEBUG_PRINTLN("value: " << value);
                if (value.find_first_of('-') != value.find_last_of('-') && value.find_last_of('-') != std::string::npos) { //there are (technically more than) 2 '-' and the last one doesnt return npos
                    try {
                        this->series_start = boost::lexical_cast<int>(value.substr(0, value.find_first_of('-'))); // = x;
                        value = value.substr(value.find_first_of('-') + 1);
                        //                    DEBUG_PRINTLN("value: " << value);
                        this->series_stop = boost::lexical_cast<int>(value.substr(0, value.find_first_of('-'))); // = y;
                        value = value.substr(value.find_first_of('-') + 1);
                        //                    DEBUG_PRINTLN("value: " << value);
                        this->series_steps = boost::lexical_cast<int>(value); // = z;
                        DEBUG_PRINTLN(series_start << " " << series_stop << " " << series_steps << " ");
                    } catch (boost::bad_lexical_cast const& e) {
                        DEBUG_PRINTLN("Error: " << e.what());
                        DEBUG_PRINTLN("Please make sure your syntax is series-x OR series-x-y-z (x,y,z arbitrary positive integer values)");
                    }
                } else {
                    this->series_steps = boost::lexical_cast<int>(value); // = x;
                }
            } else {
                this->series_steps = -1;
                DEBUG_PRINTLN("Option series-x with no integer x commited. Setting steps to " << this->series_steps << "and preventing output...");
            }
        } else if (option.compare("seed") == 0) {
            if(!value.empty())  {
                this->seed  = boost::lexical_cast<long>(value.substr(0, value.find_first_of('-')));
            } else  {
                this->seed = -1; 
                DEBUG_PRINTLN("Seed-x option was selected, but no x was given. Taking random seed instead.")
            }
        } else if (option.compare("help") == 0) {
            this->usage_help();
        } else {
            DEBUG_PRINTLN("option \"" << option << "\" not supported. Please use temp-0.01 ,conv-100000 and pre-t (values arbitrary).");
        }
    }
    if (this->temperature == -1) {
        this->temperature = 0.1;
        DEBUG_PRINTLN("No temperature specified: Setting it to: " << this->temperature);
    }
    if (this->convergence_criterion == -1) {
        this->convergence_criterion = 0.01;
        DEBUG_PRINTLN("No convergence_criterion specified: Setting it to: " << this->convergence_criterion);
    }
    if (this->check_steps == -1) {
        this->check_steps = 5000;
        DEBUG_PRINTLN("step-value not specified: Setting check_steps to: " << this->check_steps);
    } else if (this->check_steps == 0) {
        this->check_steps = INT_MAX;
    }
}

simulated_annealing_merger::simulated_annealing_merger() {
    //empty
}

simulated_annealing_merger::~simulated_annealing_merger() {
    //empty
}

void simulated_annealing_merger::merge_tiles(sharedptr<tiled_image> ti) {
    debug_file_io * file = new debug_file_io("energy.txt");

    //Mersenne twister pseudorandom number generator with prime of 2^19937-1
    boost::random::mt19937 gen((this->seed != -1) ? this->seed : std::time(NULL)); //Boost Random Generator // ternary operator
    boost::random::uniform_int_distribution<> rand_w(0, ti->get_tilecount_width() - 1);
    boost::random::uniform_int_distribution<> rand_h(0, ti->get_tilecount_height() - 1);
    boost::random::uniform_01<> dist;

    /* Prerun with Merger */
    if (!this->merge_prior.empty()) {
        if (this->merge_prior.compare("simple") == 0) {
            DEBUG_PRINTLN("Preprocessing image with simple merger");
            sharedptr<simple1d_tile_merger> my_merger(new simple1d_tile_merger());
            my_merger->merge_tiles(ti);
        } else if (this->merge_prior.compare("srncp") == 0) {
            sharedptr<abstract_reliability_calculator> rc(new reliability_calculator_variance());
            sharedptr<srncp_tile_merger> my_merger(new srncp_tile_merger(rc));
            my_merger->merge_tiles(ti);
        } else DEBUG_PRINTLN("Merger " << this->merge_prior << " supported. Please use simple or srncp");

    }

    float total_energy = 0.0f;
    float abort_parameter = 0.0f;
    float pre_energy = 0.0f;
    float post_energy = 0.0f;
    float prob_accept = 0.0f; // Acceptance probability of step. Calculated in loop
    float prob_add = 0.f; //  If prob_add > 0.5 => 2Pi is added to the tile, else -2Pi is added to the tile
    long w, h;
    int steps = 0;
    sharedptr<tile> cur_tile;

    /* Calculate the overall "energy" of every junction in the image as squared differences */
    long junc_arr_size = ti->get_size_of_junction_array();
    if (junc_arr_size == 0) {
        ti->create_all_junctions();
        junc_arr_size = ti->get_size_of_junction_array(); //Do not delete, otherwise junc_arr_size stays zero
    }

    for (int i = 0; i < junc_arr_size; i++) {
        total_energy += ti->get_junction_at(i)->calc_junction_squared_difference();
    }
    DEBUG_PRINTLN("Start total energy: " << total_energy);
    this->name_energy_prior_merging = total_energy;
    while (steps < (250 * junc_arr_size) && abort_parameter < FLT_MAX) { //TODO: Find a perfect convergence criterion...
        file->append_to_file(boost::lexical_cast<std::string>(steps) + " " + boost::lexical_cast<std::string>(total_energy) + "\n");
        steps++;

        w = rand_w(gen);
        h = rand_h(gen);
        prob_add = dist(gen);
        prob_accept = dist(gen);

        cur_tile = ti->get_tile_at(w, h);
        pre_energy = calc_energy(ti->get_neighbouring_junctions(cur_tile));

        if (prob_add > 0.5) cur_tile->add_value(2 * M_PI);
        else cur_tile->add_value(-2 * M_PI);

        post_energy = calc_energy(ti->get_neighbouring_junctions(cur_tile));

        if (prob_accept < std::exp(-(post_energy - pre_energy) / (this->temperature - (this->temperature / (250 * junc_arr_size)) * steps))) {
            total_energy += post_energy - pre_energy;

            /* Save intermediate image if chosen */
            if (this->series_steps != -1 && steps % this->series_steps == 0) { //Series saving is "on"
                if (this->series_start != -1) { //Range saving is "on"
                    if (steps >= this->series_start && steps <= this->series_stop) { //it is in range
                        this->write_intermediate(ti, boost::lexical_cast<std::string>(steps));
                    }
                } else { //Range saving is "off"
                    this->write_intermediate(ti, boost::lexical_cast<std::string>(steps));
                }
            }
            /* End saving intermediate image... */

        } else {
            //discard changes
            if (prob_add < 0.5) cur_tile->add_value(2 * M_PI);
            else cur_tile->add_value(-2 * M_PI);
        }
        if (steps % this->check_steps == 0) { //Check every 10.000 step if the total energy is the same as 10.000 steps before. True: End!
            DEBUG_PRINTLN("steps: " << steps << " at total energy: " << total_energy);
            DEBUG_PRINTLN("temperature according to accept-choice: " << (this->temperature - (this->temperature / (250 * junc_arr_size)) * steps));
            //Ternary operator:
            abort_parameter = (((abort_parameter - total_energy) < -this->convergence_criterion || (abort_parameter - total_energy) > 0) ? total_energy : FLT_MAX);
            //         if (abort_parameter != total_energy) {
            //            abort_parameter = total_energy;
            //         } else {
            //            abort_parameter = FLT_MAX;
            //         }
        }
    }
    DEBUG_PRINTLN("End total energy:  " << total_energy << " & number of steps to \"convergence\": " << steps);
    this->name_energy_after_merging = total_energy;
    this->name_steps_to_convergence = steps;

    delete file;
}

float simulated_annealing_merger::calc_energy(sharedptr<std::vector<sharedptr<tile_junction> > > junc_vc) {
    float energy = 0.0f;
    std::vector<sharedptr<tile_junction> >::iterator it;
    for (it = junc_vc->begin(); it != junc_vc->end(); ++it) {

        energy += (*it)->calc_junction_squared_difference();
    }
    return energy;
}

//float simulated_annealing_merger::keep_change(float total_energy, float pre_energy, float post_energy) {
//   total_energy -= (pre_energy - post_energy);  //Difference of this 
//}

void simulated_annealing_merger::write_intermediate(sharedptr<tiled_image> ti, std::string name) {
    sharedptr<row_major_float_image> unwrapped_img = ti->convert_to_float_image();
    std::string output_filename = "C:\\ImageSeries\\SimAnn\\" + name + ".raw";
    if (!write_image(&output_filename[0], unwrapped_img)) {
        DEBUG_PRINTLN("Error: Could not save file. \n Note: This programm has no cross-platform creation of folders (yet) supported.");
        DEBUG_PRINTLN("If you see this error message, please check if the specified output (or .\\Unwrapped\\ when no -o option called) is an EXISTING FOLDER.");
        return;
    }
}

std::string simulated_annealing_merger::get_name() {
    std::string name = "simann_";
    if (this->name_extended) {
        name += "te" + boost::lexical_cast<std::string>(this->temperature).substr(0, 5);
        name += "_c";
        name += boost::lexical_cast<std::string>(this->convergence_criterion).substr(0, 5);
        if (!this->merge_prior.empty()) name += "_" + this->merge_prior.substr(0, 3);
        name += "_#";
        name += boost::lexical_cast<std::string>(this->check_steps) + "_";
        name += boost::lexical_cast<std::string>(this->name_steps_to_convergence);
        name += "_pE";
        name += boost::lexical_cast<std::string>(this->name_energy_prior_merging).substr(0, 6);
        name += "_aE";
        name += boost::lexical_cast<std::string>(this->name_energy_after_merging).substr(0, 6);
    }
    return name;
}

void simulated_annealing_merger::usage_help() {
    DEBUG_PRINTLN("*---------------------------------------------------------------------------*");
    DEBUG_PRINTLN("Usage of the simulated annealing merger...");
    DEBUG_PRINTLN("Options");
    DEBUG_PRINTLN("temp-x :  Temperature x mostly between 1 and 0. Default at 0.1");
    DEBUG_PRINTLN("conv-x :  The method will stop (~convergence) when following is met:");
    DEBUG_PRINTLN("          0 > E1-E2 > -|x|");
    DEBUG_PRINTLN("          (E1 = Energy before flipping one tile. E2 = Energy after flipping)");
    DEBUG_PRINTLN("          DO NOT SUBMIT A SIGN! --- input x should be a small positive value!");
    DEBUG_PRINTLN("          Default is 0.1");
    DEBUG_PRINTLN("steps-x:  Convergence will be checked every x iterations. ");
    DEBUG_PRINTLN("          Values >> 1000 will be faster. ")
    DEBUG_PRINTLN("          Values around 1 to 10 may be error-prone due to local minima... ");
    DEBUG_PRINTLN("          A value of 0 will turn this criterion off!");
    DEBUG_PRINTLN("          Default is 5000");
    DEBUG_PRINTLN("pre-x  :  Prerun the x=simple or x=srncp merger (faster)");
    DEBUG_PRINTLN("saveinfo: Write temp-x, conv-x and steps for convergence into filename");
    DEBUG_PRINTLN("          Default turned off");
    DEBUG_PRINTLN("series-z:      Every z-th step the image will be saved to harddrive");
    DEBUG_PRINTLN("  OR  ");
    DEBUG_PRINTLN("series-x-y-z : Like above, but for a range:");
    DEBUG_PRINTLN("             : Start saving at image x. Stop saving before image y. Save every z-th step!");
    DEBUG_PRINTLN("seed-x  : Seed used for the random generator. Same seeds on the same picture will grant same output.");
    DEBUG_PRINTLN("          Default is random");
    DEBUG_PRINTLN("*---------------------------------------------------------------------------*");
}
