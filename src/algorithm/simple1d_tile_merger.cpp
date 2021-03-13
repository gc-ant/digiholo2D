/* 
 * File:   simple1d_tile_merger.cpp
 * Author: b.steltner, g.antonopoulos
 * 
 * Created on 11. November 2014, 10:09
 */

#include "algorithm/simple1d_tile_merger.h"
#include "tiles/tile_junction.h"

simple1d_tile_merger::simple1d_tile_merger() {
   this->left2right = true;
   this->top2bottom = true;
   this->start_horizontal = true;
}

//TODO: This constructor starts a mess if called with paramter "false". Sort the index-error out. 
simple1d_tile_merger::simple1d_tile_merger(bool left2right, bool top2bottom, bool start_horizontal) {
   this->left2right = left2right;
   this->top2bottom = top2bottom;
   this->start_horizontal = start_horizontal;
}

simple1d_tile_merger::~simple1d_tile_merger() {
}

void simple1d_tile_merger::merge_tiles(boost::shared_ptr<tiled_image> ti) {
   PRINTLN("Starting Simple1D Merger with settings: " << this->left2right << " " << this->top2bottom << " " << this->start_horizontal)
   ti->create_all_junctions();
   if (this->start_horizontal) {
      tile_unwrap1d_horizontal(ti, this->left2right);
      tile_unwrap1d_vertical(ti, this->top2bottom);
   } else {
      tile_unwrap1d_vertical(ti, this->top2bottom);
      tile_unwrap1d_horizontal(ti, this->left2right);
   }
}

/**
 * Local help function. It calculates the mean difference of the junction from tile(w,h) and the tile in the direction pos and returns it.
 * @param t smart tiled image
 * @param w current tile index width
 * @param h current tile index height
 * @param unwrap_threshold 
 * @param pos direction to which the junction should be, based on the current tile
 * @return unwrap value for the given tile
 */
float simple1d_tile_merger::calculate(boost::shared_ptr<tiled_image> ti, long w, long h, tile::rel_position pos) {
   float mean_diff = 0.0f;
   float unwrap_val = 0.0f;

   boost::shared_ptr<tile>tile = ti->get_tile_at(w, h);
   boost::shared_ptr<tile_junction> junction = ti->get_junction(tile, pos);
   mean_diff = junction->calc_junction_mean_difference();

   unwrap_val = roundf(mean_diff / (2 * M_PI));
   return (unwrap_val * (-2) * M_PI);
}

void simple1d_tile_merger::tile_unwrap1d_horizontal(boost::shared_ptr<tiled_image> ti, bool left2right) {
   int H = ti->get_tilecount_height();
   int W = ti->get_tilecount_width();
   float unwrap_val;

   for (int h = 0; h < H; h++) { //Go through every row starting from row 0 to row H-1
      if (left2right) { //Merge from left to right of the image
         for (int w = 1; w < W - 1; w++) { //Go through every column starting from 0 to W-2 (because of rel_pos::RIGHT)
            unwrap_val = calculate(ti, w, h, tile::LEFT);
            for (int i = 0; i < w; i++) { //Go through every tile in this row that was merged already
               /* Blöde Frage, aber: Warum eigentlich?? Genügt es nicht das aktuelle tile hoch/runter zu verschieben?? */
               boost::shared_ptr<tile> cur_tile = ti->get_tile_at(i, h);
               cur_tile->add_value(unwrap_val);
            }
         }
      } else { //Merge from right to left of the image
         for (int w = W - 2; w > 0; w--) { //Go through every column startning from W-1 to 1 (because of rel_pos::LEFT)
            unwrap_val = calculate(ti, w, h, tile::RIGHT);
            for (int i = W - 1; i > w; i--) {//TODO: ÜBERLEG: t1 | junc | t2. AddValue darf nicht auf t1 UND t2 addiert werden!
               //dann sehen die Bilder auch völlig gleich aus. Geht die Schleife genau andersherum, kommt murks heraus
               boost::shared_ptr<tile> cur_tile = ti->get_tile_at(i, h);
               cur_tile->add_value(unwrap_val);
            }
         }
      }
   }
}

void simple1d_tile_merger::tile_unwrap1d_vertical(boost::shared_ptr<tiled_image> ti, bool top2bottom) {
   int H = ti->get_tilecount_height();
   int W = ti->get_tilecount_width();
   float unwrap_val;

   for (int w = 0; w < W; w++) {
      if (top2bottom) { //Merge from the top to the bottom of the image
         //         if(w==1) printf("top2bottom ");
         for (int h = 0; h < H - 1; h++) {
            //            if(w==1) printf("%i ", (int) h);
            unwrap_val = calculate(ti, w, h, tile::DOWN);
            for (int i = 0; i < h + 1; i++) {
               boost::shared_ptr<tile> cur_tile = ti->get_tile_at(w, i);
               cur_tile->add_value(unwrap_val);
            }
         }
      } else { //Merge from the bottom to the top of the image
         //         if(w==1) printf("bottom2top ");
         for (int h = H - 1; h >= 1; h--) {
            //            if(w==1) printf("%i ", (int) h);
            unwrap_val = calculate(ti, w, h, tile::UP);
            for (int i = H - 1; i > h - 1; i--) {
               boost::shared_ptr<tile> cur_tile = ti->get_tile_at(w, i);
               cur_tile->add_value(unwrap_val);
            }
         }
      }
   }
}

std::string simple1d_tile_merger::get_name() {
   std::string name = "simple1d_";

   if (this->left2right == true) name += "1";
   else name += "0";
   if (this->top2bottom == true) name += "1";
   else name += "0";
   if (this->start_horizontal == true) name += "1";
   else name += "0";
   return name;
}


void simple1d_tile_merger::usage_help() {
   PRINTLN("*------------------------------------------------------------*");
   PRINTLN("Usage of the simple merger...");
   PRINTLN("No Options");
   PRINTLN("*------------------------------------------------------------*");
}