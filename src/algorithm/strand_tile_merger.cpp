/* 
 * File:   strand_tile_merger.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 25. November 2014, 11:48
 */

#include "algorithm/strand_tile_merger.h"
#include "tiles/tiled_image.h"
#include "tiles/tile_junction.h"


strand_tile_merger::strand_tile_merger() {

}

strand_tile_merger::~strand_tile_merger() {
   //empty
}

void strand_tile_merger::merge_tiles(sharedptr<tiled_image> ti) {
   ti->create_all_junctions();

   sharedptr<tilegroup> subset_U = tilegroup::create_new();
   //Insert first Block B00 into the new subset_U
   add_tile_to_group(ti->get_tile_at(0, 0), subset_U);

   long H = ti->get_tilecount_height();
   long W = ti->get_tilecount_width();
   float r = 0;

   //Iterate over every block and calculate r
   for (long w = 0; w < W; w++) {
      for (long h = 0; h < H; h++) {
         if (w == 0 && h == 0) {
            ++h; //Skip B00 as it is the setting first block
         }
         sharedptr<tile> cur_tile = ti->get_tile_at(w, h);
         //Now find every junction in which Bwh is present and the other block is in the tilegroup subset_U
         sharedptr<std::vector<sharedptr<tile_junction> > > neighbours = ti->get_neighbouring_junctions(cur_tile, subset_U);
         //Now calculate r and add it the current tile
         r = roundf(abs_diff_norm(neighbours)); //Round to integer
         cur_tile->add_value(r * 2 * M_PI); //Add value to tile and applay the value to the tiles..apply_value()
         //Add current tile to the subset_U
         add_tile_to_group(cur_tile, subset_U);
      }
   }
}

float strand_tile_merger::abs_diff_norm(sharedptr<std::vector<sharedptr<tile_junction> > > vec_junction_ptr) {
   float diff = 0.0f;
   int counter = 0;
   for (std::vector<sharedptr<tile_junction> >::iterator it = vec_junction_ptr->begin(); it != vec_junction_ptr->end(); ++it) {
      diff += (*it)->calc_junction_mean_difference();
      counter++;
   }
   return (diff / (2 * M_PI * counter));
}

std::string strand_tile_merger::get_name() {
   std::string name = "strand";
   return name;
}

void strand_tile_merger::usage_help() {
   PRINTLN("*------------------------------------------------------------*");
   PRINTLN("Usage of the strand  merger...");
   PRINTLN("No Options");
   PRINTLN("*------------------------------------------------------------*");
}