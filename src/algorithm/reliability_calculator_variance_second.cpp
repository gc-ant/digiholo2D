/* 
 * File:   reliability_calculator_variance_second_second.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 27. November 2014, 14:51
 */

#include "algorithm/reliability_calculator_variance_second.h"
#include "tiles/tile_junction.h"

reliability_calculator_variance_second::reliability_calculator_variance_second() {
    //empty
}

reliability_calculator_variance_second::~reliability_calculator_variance_second() {

}

float reliability_calculator_variance_second::calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile> t) {
    std::vector<sharedptr<tile_junction> > vc_junc;
    vc_junc.push_back(ti->get_junction(t, tile::UP));
    vc_junc.push_back(ti->get_junction(t, tile::DOWN));
    vc_junc.push_back(ti->get_junction(t, tile::LEFT));
    vc_junc.push_back(ti->get_junction(t, tile::RIGHT));

    float reliability = 0.f;
    float inv_max_quarter = 4 / FLT_MAX;

    for (sharedptr<tile_junction> junction : vc_junc) {
        if (junction) {
            if (junction->get_variance() < inv_max_quarter) {
                reliability += FLT_MAX / 4;
            } else {
                reliability += 1 / junction->get_variance();
            }
        }
    }
    //NOTE: get_variance returns only the private junction.variance and does NOT check if it is initialised. Make use of 
    //      calculate_junction_variance before calling this...

    return reliability;
}

float reliability_calculator_variance_second::calculate_reliability(boost::shared_ptr<tiled_image> ti, boost::shared_ptr<tile_junction> junc) {
   float rel_1 = this->calculate_reliability(ti, junc->get_first().lock());
   float rel_2 = this->calculate_reliability(ti, junc->get_second().lock());
   return (rel_1 * rel_2);
}

void reliability_calculator_variance_second::init_junctions(sharedptr<tiled_image> ti)   {
    long end = ti->get_size_of_junction_array();
    if(end == 0)    {
        ti->create_all_junctions(); 
        end = ti->get_size_of_junction_array(); 
    }
    for(int i = 0; i < end; i++)    {
        ti->get_junction_at(i)->calc_junction_variance();
    }
}