#include "algorithm/simulated_annealing_floodfill_merger.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_01.hpp>

#include "debug/debug_time.h"
#include "debug/debug_file_io.h"

#include "algorithm/reliability_calculator_variance.h"
#include "algorithm/reliability_calculator_variance_second.h"
#include "algorithm/srncp_tile_merger.h"
#include "algorithm/simple1d_tile_merger.h"
#include "tiles/tile_junction.h"
#include "image/in_out.h"
#include <cmath>
#include <unordered_map>

simulated_annealing_floodfill_merger::simulated_annealing_floodfill_merger(int check_steps, float convergence_criterion, std::string merge_prior, float name_energy_after_merging, float name_energy_prior_merging, bool name_extended, int name_steps_to_convergence, long seed, int series_start, int series_steps, int series_stop, float temperature, float good_mean, float good_variance) :
abstract_tile_merger(), check_steps(check_steps), convergence_criterion(convergence_criterion), temperature(temperature), merge_prior(merge_prior), name_steps_to_convergence(name_steps_to_convergence), name_extended(name_extended), name_energy_prior_merging(name_energy_prior_merging), name_energy_after_merging(name_energy_after_merging), series_start(series_start), series_stop(series_stop), series_steps(series_steps), seed(seed), good_mean(good_mean), good_variance(good_variance) {
    //empty
}

simulated_annealing_floodfill_merger::simulated_annealing_floodfill_merger() {
    //empty
}

simulated_annealing_floodfill_merger::~simulated_annealing_floodfill_merger() {
    //empty
}

simulated_annealing_floodfill_merger::simulated_annealing_floodfill_merger(std::vector<std::string> msettings) {
    /* Pre-initialization of the variables with default-values */
    this->check_steps = -1;
    this->convergence_criterion = 0.9;
    this->good_mean = 0.1;
    this->good_variance = 0.2;
    this->merge_prior = "";
    this->name_energy_after_merging = 0;
    this->name_energy_prior_merging = 0;
    this->name_extended = false;
    this->name_steps_to_convergence = 0;
    this->seed = -1;
    this->series_start = -1;
    this->series_steps = -1;
    this->series_stop = -1;
    this->temperature = 0.1;

    try {
        /* Change datatype of vc_string to map_string: option name is the key to the value being the value of the option */
        std::unordered_map<std::string, std::string> map_string;
        map_string.reserve(msettings.size());
        std::string::size_type seperator;
        for (std::string option : msettings) {
            seperator = option.find_first_of('-');
            map_string.insert(std::make_pair<std::string, std::string>(option.substr(0, seperator), option.substr(seperator + 1)));
        }

        /* Grab the options out of the map */
        std::unordered_map<std::string, std::string>::const_iterator it_option;

        /* Assign steps option to check_steps */
        it_option = map_string.find("steps"); //find entry "steps" in the map. If no "steps" is found, returns past-the-end-iterator
        if (it_option != map_string.end() && !it_option->second.empty()) this->check_steps = boost::lexical_cast<int>(it_option->second);

        /* Assign conv option to convergence_criterion */
        it_option = map_string.find("conv");
        if (it_option != map_string.end() && !it_option->second.empty()) this->convergence_criterion = boost::lexical_cast<float>(it_option->second);

        /* Assign mean option to good_mean */
        it_option = map_string.find("mean");
        if (it_option != map_string.end() && !it_option->second.empty()) this->good_mean = boost::lexical_cast<float>(it_option->second);

        /* Assign variance option to good_variance */
        it_option = map_string.find("variance");
        if (it_option != map_string.end() && !it_option->second.empty()) this->good_variance = boost::lexical_cast<float>(it_option->second);

        /* Assign pre option to merge_prior */
        it_option = map_string.find("pre");
        if (it_option != map_string.end() && !it_option->second.empty()) this->merge_prior = it_option->second;

        /* Assign saveinfo option to name_extended */
        if (map_string.count("saveinfo") != 0) this->name_extended = true;

        /* Assign seed option to seed */
        it_option = map_string.find("seed");
        if (it_option != map_string.end() && !it_option->second.empty()) this->seed = boost::lexical_cast<long>(it_option->second);

        /* Assign series option to series_steps (and *_start and *_end) */
        it_option = map_string.find("series");
        if (it_option != map_string.end() && !it_option->second.empty()) {
            if (it_option->second.find_first_of('-') == std::string::npos) {
                this->series_steps = boost::lexical_cast<long>(it_option->second);
                this->series_start = 0;
                this->series_stop = INT_MAX;
            } else {
                seperator = it_option->second.find_first_of('-'); //it_option should contain x-y-z
                this->series_start = boost::lexical_cast<long>(it_option->second.substr(0, seperator)); //is x now
                std::string temp_string = it_option->second.substr(seperator + 1); //this contains "y-z" now
                seperator = temp_string.find_first_of('-');
                if (seperator != std::string::npos) {
                    this->series_stop = boost::lexical_cast<long>(temp_string.substr(0, seperator)); //is y now
                    this->series_steps = boost::lexical_cast<long>(temp_string.substr(seperator + 1)); //is z now
                }
            }
        }

        /* Assign temp to temperature */
        it_option = map_string.find("temp");
        if (it_option != map_string.end() && !it_option->second.empty()) this->temperature = boost::lexical_cast<float>(it_option->second);

        /* Call help */
        if (map_string.count("help") != 0) this->usage_help();
    } catch (const boost::bad_lexical_cast &) {
        DEBUG_PRINTLN("*---------------------------ERROR-------------------------------------------*");
        DEBUG_PRINTLN("Error: Bad lexical cast from Simulated Annealing Floodfill Merger Constructor...");
        DEBUG_PRINTLN("Trying to merge with these options:")
    }

    this->show_options();
}

void simulated_annealing_floodfill_merger::merge_tiles(sharedptr<tiled_image> ti) {
    long H = ti->get_tilecount_height();
    long W = ti->get_tilecount_width();

    //Mersenne twister pseudorandom number generator with prime of 2^19937-1
    boost::random::mt19937 gen((this->seed != -1) ? this->seed : (int) std::time(NULL)); //Boost Random Generator // ternary operator
    boost::random::uniform_int_distribution<> iw(0, W - 1);
    boost::random::uniform_int_distribution<> ih(0, H - 1);
    boost::random::uniform_01<> dist;
    DEBUG_PRINTLN("Seed: " << this->seed);

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
    long steps = 0;

    sharedptr<tilegroup> tg_open = tilegroup::create_new();
    sharedptr<tilegroup> tg_closed = tilegroup::create_new();
    sharedptr<abstract_reliability_calculator> rc(new reliability_calculator_variance());
    rc->init_junctions(ti);

    long junc_arr_size = ti->get_size_of_junction_array();

    if (junc_arr_size == 0) DEBUG_PRINTLN("No junction initiliazed. Error, should be done via rc_variance");

    for (int i = 0; i < junc_arr_size; i++) {
        this->name_energy_prior_merging += ti->get_junction_at(i)->calc_junction_squared_difference(); //TODO: Substitute through energy from tilegroup_junctions...
    }

    if (this->check_steps == -1) {
        this->check_steps = junc_arr_size / 6;
        DEBUG_PRINTLN("step-value not specified: Setting check_steps to: " << this->check_steps);
    } else if (this->check_steps == 0) {
        this->check_steps = INT_MAX;
    }

    while (steps++ < this->check_steps - 50 && tg_open->size() < this->convergence_criterion * W * H) {
        tg_open.reset();
        tg_closed.reset();
        tg_open = tilegroup::create_new();
        tg_closed = tilegroup::create_new();

        this->add_value_and_check(ti, ti->get_tile_at(iw(gen), ih(gen)), dist(gen), dist(gen), tg_open, tg_closed, rc, junc_arr_size, steps);
    }
    int temp_counter = 0;

    for (int k = 1; k < 4; k++) {
        while (tg_open->size() < this->convergence_criterion * W * H && temp_counter++ <= 5) {
            tg_open.reset();
            tg_closed.reset();
            tg_open = tilegroup::create_new();
            tg_closed = tilegroup::create_new();

            this->floodfill(ti, ti->get_tile_at(iw(gen), ih(gen)), tg_open, tg_closed, rc);
        }
        temp_counter = 20;
        tg_open->add_value(6 * M_PI);
        tg_closed->add_value(6 * M_PI);

        while (temp_counter-- >= 0) {
            tg_open.reset();
            tg_closed.reset();
            tg_open = tilegroup::create_new();
            tg_closed = tilegroup::create_new();

            add_value_and_check(ti, ti->get_tile_at(iw(gen), ih(gen)), dist(gen), dist(gen), tg_open, tg_closed, rc, junc_arr_size, ++steps);
            //            this->write_intermediate(ti, "t"+boost::lexical_cast<std::string>(steps));
        }
        for (long i = 0; i < junc_arr_size; i++) {
            this->name_energy_after_merging += ti->get_junction_at(i)->calc_junction_squared_difference();
        }
    }
    this->name_steps_to_convergence = steps;
}

void simulated_annealing_floodfill_merger::add_value_and_check(sharedptr<tiled_image> ti, sharedptr<tile> cur_tile, float prop_add, float prop_accept, sharedptr<tilegroup> tg_open, sharedptr<tilegroup> tg_closed, sharedptr<abstract_reliability_calculator> rc, long junc_arr_size, long step) {
    this->floodfill(ti, cur_tile, tg_open, tg_closed, rc);
    //    DEBUG_PRINTLN("step: " << step << " #tg_open: " << tg_open->size() << " #tg_closed: " << tg_closed->size() << " prop_add: " << prop_add << " prop_accept: " << prop_accept << " junc_arr_size: " << junc_arr_size);
    float pre_energy = 0.0f;
    float post_energy = 0.0f;

    for (long i = 0; i < junc_arr_size; i++) {
        pre_energy += ti->get_junction_at(i)->calc_junction_squared_difference();
    }
    if (prop_add > 0.5) {
        tg_open->add_value(2 * M_PI);
        tg_closed->add_value(2 * M_PI);
    } else {
        tg_open->add_value(-2 * M_PI);
        tg_closed->add_value(-2 * M_PI);
    }
    for (long i = 0; i < junc_arr_size; i++) {
        post_energy += ti->get_junction_at(i)->calc_junction_squared_difference();
    }
    if (prop_accept <= std::exp(-(post_energy - pre_energy) / (this->temperature - (step * this->temperature / (250 * junc_arr_size))))) {
        //ACCEPT GUESS

        /* Save intermediate image if chosen */
        if (this->series_steps != -1 && step % this->series_steps == 0) { //Series saving is "on"
            if (this->series_start != -1) { //Range saving is "on"
                if (step >= this->series_start && step <= this->series_stop) { //it is in range
                    this->write_intermediate(ti, boost::lexical_cast<std::string>(step));
                }
            } else { //Range saving is "off"
                this->write_intermediate(ti, boost::lexical_cast<std::string>(step));
            }
        }
        /* End saving intermediate image... */

    } else {
        //DECLINE GUESS
        if (prop_add < 0.5) {
            tg_open->add_value(2 * M_PI);
            tg_closed->add_value(2 * M_PI);
        } else {
            tg_open->add_value(-2 * M_PI);
            tg_closed->add_value(-2 * M_PI);
        }
    }
}

void simulated_annealing_floodfill_merger::floodfill(sharedptr<tiled_image> ti, sharedptr<tile> t, sharedptr<tilegroup> tg_open, sharedptr<tilegroup> tg_closed, sharedptr<abstract_reliability_calculator> rc) {
    //if (t->has_tilegroup()) return; //if tile is in either closed or open, it has been computed before
    //    DEBUG_PRINTLN("var: " << std::pow(1 / rc->calculate_reliability(ti, t), 2.0f));
    //with value 0.11 the tile is measured as "good" if the sum of all junction variances are below 3 //Todo value seems to be too low, maybe more like 0.9??
    sharedptr<std::vector<sharedptr<tile_junction> > > vc_junc = ti->get_neighbouring_junctions(t);
    for (sharedptr<tile_junction> junc : *vc_junc) {
        junc->calc_junction_variance(); //has to be called prior to using it in rc_variance
    }

    if (std::pow(1 / rc->calculate_reliability(ti, t), 2.0f) < 0.1) { // Good tile = High reliability. Therefor (1/rc)^2 has to be smaller than ...
        //good tile
        tg_open->add_unique_tile(t, tg_open);

        long iw = t->get_posW();
        long W = ti->get_tilecount_width();
        long ih = t->get_posH();
        long H = ti->get_tilecount_height();

        sharedptr<tile> neighbour_tile;



        if (iw > 0) { //there has to be a left tile
            //todo 4x, maybe pack that in a function 
            this->recur_floodfile(ti, t, ti->get_tile_at(iw - 1, ih), tg_open, tg_closed, rc);
        }
        if (iw < W - 1) { //there has to be a right tile
            this->recur_floodfile(ti, t, ti->get_tile_at(iw + 1, ih), tg_open, tg_closed, rc);
        }
        if (ih > 0) { //tile on top
            this->recur_floodfile(ti, t, ti->get_tile_at(iw, ih - 1), tg_open, tg_closed, rc);
        }
        if (ih < H - 1) {
            this->recur_floodfile(ti, t, ti->get_tile_at(iw, ih + 1), tg_open, tg_closed, rc);
        }
    } else {
        //bad tile
        tg_closed->add_unique_tile(t, tg_closed);
    }
}

void simulated_annealing_floodfill_merger::recur_floodfile(sharedptr<tiled_image> ti, sharedptr<tile> t, sharedptr<tile> neighbour_tile, sharedptr<tilegroup> tg_open, sharedptr<tilegroup> tg_closed, sharedptr<abstract_reliability_calculator> rc) {
    float accept_value = 0.2;
    float mean_diff = ti->get_junction(t, neighbour_tile)->calc_junction_mean_difference();

    if (!neighbour_tile->has_tilegroup() && mean_diff < accept_value && mean_diff > -accept_value) {
        this->floodfill(ti, neighbour_tile, tg_open, tg_closed, rc);
    }
}

float simulated_annealing_floodfill_merger::calc_energy(sharedptr<std::vector<sharedptr<tile_junction> > > junc_vc) {
    return 0.0f;
}

//float simulated_annealing_floodfill_merger::keep_change(float total_energy, float pre_energy, float post_energy) {
//   total_energy -= (pre_energy - post_energy);  //Difference of this 
//}

void simulated_annealing_floodfill_merger::write_intermediate(sharedptr<tiled_image> ti, std::string name) {
    sharedptr<row_major_float_image> unwrapped_img = ti->convert_to_float_image();
    std::string output_filename = "C:\\ImageSeries\\SimAnnFlood\\" + name + ".raw";
    if (!write_image(&output_filename[0], unwrapped_img)) {
        DEBUG_PRINTLN("Error: Could not save file. \n Note: This programm has no cross-platform creation of folders (yet) supported.");
        DEBUG_PRINTLN("If you see this error message, please check if the specified output (or .\\Unwrapped\\ when no -o option called) is an EXISTING FOLDER.");
        return;
    }
}

std::string simulated_annealing_floodfill_merger::get_name() {
    std::string name = "simannflood_";
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

void simulated_annealing_floodfill_merger::show_options() {
    DEBUG_PRINTLN("*-----------------------Merger options--------------------------------------*");
    DEBUG_PRINTLN("(Values of -1 (or 0) are (mostly) default and will be set in the programm later)")
    DEBUG_PRINTLN("steps: " << this->check_steps);
    DEBUG_PRINTLN("conv: " << this->convergence_criterion);
    DEBUG_PRINTLN("temp: " << this->temperature);
    DEBUG_PRINTLN("pre: " << this->merge_prior);
    DEBUG_PRINTLN("saveinfo: " << this->name_extended);
    DEBUG_PRINTLN("series_start: " << this->series_start);
    DEBUG_PRINTLN("series_stop: " << this->series_stop);
    DEBUG_PRINTLN("series_steps: " << this->series_steps);
    DEBUG_PRINTLN("seed: " << this->seed);
    DEBUG_PRINTLN("mean: " << this->good_mean);
    DEBUG_PRINTLN("variance: " << this->good_variance);
    DEBUG_PRINTLN("*---------------------------------------------------------------------------*");
}

void simulated_annealing_floodfill_merger::usage_help() {
    DEBUG_PRINTLN("*---------------------------------------------------------------------------*");
    DEBUG_PRINTLN("Usage of the simulated annealing merger with floodfill...");
    DEBUG_PRINTLN("Options");
    DEBUG_PRINTLN("temp-x :  Temperature x mostly between 1 and 0. Default at 0.1");
    DEBUG_PRINTLN("conv-x :  Algorithm will start ending-procedure when 100*x% of the image are connected.");
    DEBUG_PRINTLN("          x must be between 0 and 1. Default is 0.9");
    //    DEBUG_PRINTLN("          (E1 = Energy before flipping one tile. E2 = Energy after flipping)");
    //    DEBUG_PRINTLN("          DO NOT SUBMIT A SIGN! --- input x should be a small positive value!");
    //    DEBUG_PRINTLN("          Default is 0.1");
    DEBUG_PRINTLN("variance-x:  Define maximum value of the tile:reliability to be trustworthy");
    DEBUG_PRINTLN("          Default is 0.2");
    DEBUG_PRINTLN("mean-x:   Define maximum value of the junction:mean to be NO border between two tiles");
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
