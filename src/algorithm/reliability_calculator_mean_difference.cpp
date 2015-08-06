/* 
 * File:   reliability_calculator_mean_difference.cpp
 * Author: b.steltner
 *
 * Created on 25. November 2014, 11:48
 */

#include "algorithm/reliability_calculator_mean_difference.h"
#include "tiles/tile_junction.h"
#include <cfloat>
#include <cmath>

reliability_calculator_mean_difference::reliability_calculator_mean_difference() {

}

reliability_calculator_mean_difference::~reliability_calculator_mean_difference() {

}

float reliability_calculator_mean_difference::calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t) {
    boost::shared_ptr<tile_junction> junction_up = ti->get_junction(t, tile::UP);
    boost::shared_ptr<tile_junction> junction_down = ti->get_junction(t, tile::DOWN);
    boost::shared_ptr<tile_junction> junction_left = ti->get_junction(t, tile::LEFT);
    boost::shared_ptr<tile_junction> junction_right = ti->get_junction(t, tile::RIGHT);

    float reliability = 1.0f;

    if (junction_up) {
        reliability *= reliability_function(junction_up->calc_junction_mean_difference());
    } //TODO: No else here: Maybe provide some penalty for beeing a block on the edge of an image.
    if (junction_down) {
        reliability *= reliability_function(junction_down->calc_junction_mean_difference());
    }
    if (junction_left) {
        reliability *= reliability_function(junction_left->calc_junction_mean_difference());
    }
    if (junction_right) {
        reliability *= reliability_function(junction_right->calc_junction_mean_difference());
    }
    //example: best: reliability = 1.0f * 1 * 1 * 1 * 1 = 1
    //exmaple: reliability = 1.0f * 0.96 * 0.2 * 0.5 = 0.096 (block on the edge => only 3 junctions)
    //example: worst reliability = 0; 

    return reliability;
}

//JUNCTION

float reliability_calculator_mean_difference::calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> junc) {
    //   DEBUG_PRINTLN("rc_sec_dif::calc_rel(junction)");
    float rel_1 = this->calculate_reliability(ti, junc->get_first().lock());
    float rel_2 = this->calculate_reliability(ti, junc->get_second().lock());
    return (rel_1 * rel_2);
}

float reliability_calculator_mean_difference::reliability_function(float value) {
    return std::pow(cos(value / 2), 8.0); //1: best, 0: worst differences
    /* The function (cos(x/2))^4 was chosen because of it having its maxima at multiples of 2pi
     * and minima at (2n+1)pi (n integer). The power of 4 was chosen so that the function has 
     * steeper peaks, therefore only a small amount of values are receiving values near 1.
     */
    /* TODO: Is the power of 4 neccessary? I can only imagine errors with round, otherwise 
     * a power of 2, 4, 6.. has no other effect than increasing runtime, because: 
     * Say we have values 0, 0.1, 0.2. Inserted into this function with power of 2 we get in return:
     * 1, 0.99, 0.96. With power of 4 we get: 
     * 1, 0.98, 0.92. 
     * In a sorted list however the distance of these numbers doenst matter, as they will be sorted in the same way.
     * 
     * NOTE: MAYBE: Adding these values may make higher powers useful, making a bad value more influential...
     */
}

void reliability_calculator_mean_difference::init_junctions(sharedptr<tiled_image> ti) {
    long end = ti->get_size_of_junction_array();
    if (end == 0) {
        ti->create_all_junctions();
        end = ti->get_size_of_junction_array();
    }
    for (int i = 0; i < end; i++) {
        ti->get_junction_at(i)->calc_junction_mean_difference();
    }
}
