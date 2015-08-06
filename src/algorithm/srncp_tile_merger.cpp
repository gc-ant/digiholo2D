/* 
 * File:   srncp_tile_merger.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 25. November 2014, 11:48
 */

#include "algorithm/srncp_tile_merger.h"
#include "tiles/tile.h"
#include "tiles/tile_junction.h"

#include <forward_list>

#include "image/in_out.h"
#include "algorithm/reliability_calculator_random.h"
#include "algorithm/reliability_calculator_variance.h"
#include "algorithm/reliability_calculator_variance_second.h"
#include <algorithm>

#include "debug/debug_time.h"
#include "debug/debug_file_io.h"

srncp_tile_merger::srncp_tile_merger(std::vector<std::string> msettings) {
    this->series_start = -1;
    this->series_stop = -1;
    this->series_steps = -1;
    this->name_extend = "var"; 

    std::string option, value;
    size_t seperator;
    for (std::string parameter : msettings) {
        seperator = parameter.find_first_of('-');
        option = parameter.substr(0, seperator);
        value = (seperator != std::string::npos ? parameter.substr(parameter.find_first_of('-') + 1) : "");
        if (option.compare("variance") == 0) {
            sharedptr<abstract_reliability_calculator> rc(new reliability_calculator_variance());
            this->rc = rc;
            this->name_extend = "var"; 
        } else if (option.compare("random") == 0) {
            sharedptr<abstract_reliability_calculator> rc(new reliability_calculator_random());
            this->rc = rc; 
            this->name_extend = "rand"; 
        } else if (option.compare("variance_2") == 0) {
            sharedptr<abstract_reliability_calculator> rc(new reliability_calculator_variance_second());
            this->rc = rc; 
            this->name_extend = "var2"; 
        } else if (option.compare("series") == 0) {
            DEBUG_PRINTLN("Series-option was selected. DEFAULT printout is to C:\\ImageSeries\\SRNCP\\ . Please make sure this directory exists...")
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
        } else {
            DEBUG_PRINTLN("Error: Option " << option << " is not valid. Use variance or series");
        }
    }
    if (!this->rc) { //boost pointer returns false => not assigned
        DEBUG_PRINTLN("No reliabilty calculator chosen. Taking rc_variance...")
        sharedptr<abstract_reliability_calculator> rc(new reliability_calculator_variance());
        this->rc = rc;
    }
}

srncp_tile_merger::srncp_tile_merger(sharedptr<abstract_reliability_calculator> rc) {
    this->rc = rc;
}

srncp_tile_merger::~srncp_tile_merger() {

}

void srncp_tile_merger::initialise_merger(sharedptr<tiled_image> ti, sharedptr<std::multimap<float, sharedptr<tile_junction>, std::greater<float> > > junc_rel) {
    /* Initiliase all junctions
     * Declare some sizes, used by the for-loop, for better readability
     * Iterate over every junction in the sti junction array:
     *       Get the junction 
     *       Calculate the Variance of this junction
     *       Insert the pair of calculated reliability (of the junction) and the junction itself into the multimap
     * (Note: Calculate the variance via calc_junction_variance in this for-loop once and use get_variance in the reliability calculator.)
     * End
     */
    ti->create_all_junctions();
    sharedptr<tile_junction> temp_junc;
    long junc_arr_size = ti->get_size_of_junction_array(); 
    rc->init_junctions(ti); 

    for (long i = 0; i < junc_arr_size; i++) { //No iterator, because sti returns only single elements, no iterator nor complete vector
        temp_junc = ti->get_junction_at(i);
        junc_rel->insert(std::make_pair(this->rc->calculate_reliability(ti, temp_junc), temp_junc));
//        DEBUG_PRINTLN(this->rc->calculate_reliability(ti, temp_junc)); 
//        if(i == 100) exit(0); 
    }
}

void srncp_tile_merger::merge_tiles(sharedptr<tiled_image> ti) {
    /* Initialise: Fill multimap with junctions in order of sinking reliability */
    sharedptr<std::multimap<float, sharedptr<tile_junction>, std::greater<float> > > map_junction(new std::multimap<float, sharedptr<tile_junction>, std::greater<float> >);
//    sharedptr<std::multimap<float, sharedptr<tile_junction>, std::greater<float> > > map_junction(new std::multimap<float, sharedptr<tile_junction>, std::greater<float> >);
    this->initialise_merger(ti, map_junction);
    
    /* Initialise a vector for saving the (empty) tilegroups, preventing the memory range from being deleted */
    std::vector<sharedptr<tilegroup> >save_tilegroups;

    /* Declare variables used in the for-loop */
    std::multimap<float, sharedptr<tile_junction>, std::greater<float> >::iterator it;
    sharedptr<tile_junction> junc;
    sharedptr<tile> t1;
    sharedptr<tile> t2;
    sharedptr<tilegroup> g1;
    sharedptr<tilegroup> g2;
    float mean_diff, unwrap_val;
    int counter = -1;
    for (it = map_junction->begin(); it != map_junction->end(); ++it) { //Iterate over every junction

        if (this->series_steps != -1 && ++counter % this->series_steps == 0) { //Series saving is "on"
            if (this->series_start != -1) { //Range saving is "on"
                if (counter >= this->series_start && counter <= this->series_stop) { //it is in range
                    this->write_intermediate(ti, boost::lexical_cast<std::string>(counter));
                }
            } else { //Range saving is "off"
                this->write_intermediate(ti, boost::lexical_cast<std::string>(counter));
            }
        }

        junc = it->second;
        t1 = junc->get_first().lock(); //note: second means second T of pair(float, sharePtr<tile>)
        t2 = junc->get_second().lock();

        if (t1->has_tilegroup()) g1 = t1->get_tilegroup();
        else g1.reset();
        if (t2->has_tilegroup()) g2 = t2->get_tilegroup();
        else g2.reset();

        mean_diff = junc->calc_junction_mean_difference();
        unwrap_val = roundf(mean_diff / (2 * M_PI)) * 2 * M_PI;

        if (t1->has_tilegroup()) {
            if (t2->has_tilegroup()) {
                //both have g
                if (g1->size() > g2->size()) {
                    g2->add_value(+unwrap_val);
                    //               save_tilegroups.push_back(g2); //merge tilegroups will empty g2. To prevent the memory range from being deleted, save 1 strong reference...
                    merge_tilegroups(g2, g1); //Merge g2 into g1
                } else {
                    g1->add_value(-unwrap_val);
                    //               save_tilegroups.push_back(g1);
                    merge_tilegroups(g1, g2);
                }
            } else {
                //only t1 has g
                t2->add_value(+unwrap_val);
                add_tile_to_group(t2, g1);
            }
        } else {
            if (t2->has_tilegroup()) {
                //only t2 has g
                t1->add_value(-unwrap_val);
                add_tile_to_group(t1, g2);
            } else {
                //none has g
                g1 = tilegroup::create_new();
                save_tilegroups.push_back(g1);
                t2->add_value(+unwrap_val);
                add_tile_to_group(t1, g1);
                add_tile_to_group(t2, g1);
            }
        }
    }
    //   DEBUG_PRINTLN("#tilegroups: " << save_tilegroups.size());
}

void srncp_tile_merger::write_intermediate(sharedptr<tiled_image> ti, std::string name) {
    sharedptr<row_major_float_image> unwrapped_img = ti->convert_to_float_image();
    std::string output_filename = "C:\\ImageSeries\\SRNCP\\" + name + ".raw";
    if (!write_image(&output_filename[0], unwrapped_img)) {
        DEBUG_PRINTLN("Error: Could not save file. \n Note: This programm has no cross-platform creation of folders (yet) supported.");
        DEBUG_PRINTLN("If you see this error message, please check if the specified output (or .\\Unwrapped\\ when no -o option called) is an EXISTING FOLDER.");
        return;
    }
}

std::string srncp_tile_merger::get_name() {
    std::string name = "srncp_";
    return name+name_extend;
}

void srncp_tile_merger::usage_help() {
    DEBUG_PRINTLN("*------------------------------------------------------------*");
    DEBUG_PRINTLN("Usage of the srncp tile merger...");
    DEBUG_PRINTLN("Options:");
    DEBUG_PRINTLN("variance     :  Variance reliabilty calculator will be used (default)");
    DEBUG_PRINTLN("series-z     : Every z-th step the image will be saved to harddrive");
    DEBUG_PRINTLN("  OR  ");
    DEBUG_PRINTLN("series-x-y-z : Like above, but for a range:");
    DEBUG_PRINTLN("             : Start saving at image x. Stop saving before image y. Save every z-th step!");
    DEBUG_PRINTLN("*------------------------------------------------------------*");
}