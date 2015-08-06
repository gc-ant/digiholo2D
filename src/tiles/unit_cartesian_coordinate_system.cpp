#include "tiles/unit_cartesian_coordinate_system.h"

unit_cartesian_coordinate_system::unit_cartesian_coordinate_system()
{
    this->tile_width=0;
    this->tile_height=0;
    this->delta_x = 0.f;
    this->delta_y = 0.f;
}

void unit_cartesian_coordinate_system::init(sharedptr<tile> t)
{
    this->tile_width = t->get_width();
    this->tile_height = t->get_height();
    this->delta_x = 1.f/(float(tile_width-1));
    this->delta_y = 1.f/(float(tile_height-1));
}

unit_cartesian_coordinate_system::~unit_cartesian_coordinate_system()
{
    //nothing to do
}


float unit_cartesian_coordinate_system::calc_x_at(long iw, long ih)
{
    return (float(iw)*delta_x);
}

float unit_cartesian_coordinate_system::calc_y_at(long iw, long ih)
{
     return (float(ih)*delta_y);
}

float unit_cartesian_coordinate_system::calc_delta_x(long iw, long ih)
{
    return this->get_delta_x();
}

float unit_cartesian_coordinate_system::calc_delta_y(long iw, long ih)
{
    return this->get_delta_y();
}

float unit_cartesian_coordinate_system::get_delta_x()
{
    return this->delta_x;
}

float unit_cartesian_coordinate_system::get_delta_y()
{
    return this->delta_y;
}
