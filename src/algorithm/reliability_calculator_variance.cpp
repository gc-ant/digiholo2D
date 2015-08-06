/* 
 * File:   reliability_calculator_variance.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 27. November 2014, 14:51
 */

#include "algorithm/reliability_calculator_variance.h"
#include "tiles/tile_junction.h"

const float SQRT_FLT_MAX = std::sqrt(FLT_MAX); 

reliability_calculator_variance::reliability_calculator_variance() {
    //empty
    DEBUG_PRINTLN("sqrt of flt max: " <<SQRT_FLT_MAX);
}

reliability_calculator_variance::~reliability_calculator_variance() {
    //empty
}

float reliability_calculator_variance::calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t) {
    std::vector<sharedptr<tile_junction> > vc_junc;
    vc_junc.push_back(ti->get_junction(t, tile::UP));
    vc_junc.push_back(ti->get_junction(t, tile::DOWN));
    vc_junc.push_back(ti->get_junction(t, tile::LEFT));
    vc_junc.push_back(ti->get_junction(t, tile::RIGHT));

    float sum_variance = 0.f;
    float num_junctions = 0.f;
    for (sharedptr<tile_junction> junction : vc_junc) {
        if (junction)
        {
            num_junctions++;
            sum_variance += junction->get_variance();
        }
    }
    //Note: Catch too small reliability values to prevent overflow in the calc_rel(tiled_image, JUNCTION) method. Therefore we 
    //have to limit the return to maximum sqrt(FLT_MAX).
    if (num_junctions/sum_variance > SQRT_FLT_MAX) return SQRT_FLT_MAX; //
    else return num_junctions / sum_variance;
}

float reliability_calculator_variance::calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> junc) {
    float rel_1 = this->calculate_reliability(ti, junc->get_first().lock());
    float rel_2 = this->calculate_reliability(ti, junc->get_second().lock());
    return (rel_1 * rel_2);
}

void reliability_calculator_variance::init_junctions(sharedptr<tiled_image> ti) {
    long end = ti->get_size_of_junction_array();
    if (end == 0) {
        ti->create_all_junctions();
        end = ti->get_size_of_junction_array();
    }
    for (int i = 0; i < end; i++) {
        ti->get_junction_at(i)->calc_junction_variance();
    }
}