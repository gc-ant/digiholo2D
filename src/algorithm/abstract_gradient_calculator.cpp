/* 
 * File:   abstract_gradient_calculator.cpp
 * Author: g. antonopoulos, b.steltner
 *
 * Created on 30. Dezember 2014, 14:03
 */

#include "algorithm/abstract_gradient_calculator.h"
#include "tiles/unit_cartesian_coordinate_system.h"
#include "tiles/tile.h"

sharedptr<std::vector<tile> > abstract_gradient_calculator::get_gradient_tile(sharedptr<tile> t)
{
    long W = t->get_width();
    long H = t->get_height();

    sharedptr<std::vector<float> > gradient = this->get_gradient_floats(t);
    if (gradient->size() != (unsigned) 2 * W * H) PRINTLN("Vector size is: " << gradient->size() << " and should be: " << 2 * W * H);

    //   std::vector<float> gradient_x(gradient->begin(), gradient->begin() + W * H);
    //   //Iterators behave "like" pointers. Iterator + 10 => Iterator on position n+10 or n+9 (???))
    //   std::vector<float> gradient_y(gradient->begin() + W * H + 1, gradient->end());

    sharedarray<float> gradient_x(new float[W * H]);
    sharedarray<float> gradient_y(new float[W * H]);

    for(long i = 0; i < W*H; i++)
    {
        gradient_x[i] = gradient->at(i);
    }

    for(long i = 0; i < W*H; i++)
    {
        gradient_y[i] = gradient->at(W*H+i);
    }

    sharedptr<std::vector<tile > > gradient_tiles(new std::vector<tile>);
    gradient_tiles->emplace_back(gradient_x, W, H, t->get_posW(), t->get_posH());
    gradient_tiles->emplace_back(gradient_y, W, H, t->get_posW(), t->get_posH());
    return gradient_tiles;
}