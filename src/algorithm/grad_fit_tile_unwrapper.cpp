/* 
 * File:   grad_fit_tile_unwrapper.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 5. Dezember 2014, 11:53
 */

#include "algorithm/grad_fit_tile_unwrapper.h"
#include "tiles/tile.h"
#include "algorithm/wrap.h"


//grad_fit_tile_unwrapper::grad_fit_tile_unwrapper() {
//}
//
//grad_fit_tile_unwrapper::~grad_fit_tile_unwrapper() {
//
//}
//

void grad_fit_tile_unwrapper::unwrap(sharedptr<tile> t) {
   int H = t->get_height();
   int W = t->get_width();

   float a = 0.f;
   float b = 0.f;
   float c = 0.f;
   //Calcualate the mean of the gradient in iw-direction
   for (int ih = 0; ih < H; ih++) {
      for (int iw = 0; iw < W - 1; iw++) {
         a += wrapToPi(t->get_raw_value_at(iw + 1, ih) - t->get_raw_value_at(iw, ih)); //Tile has no offset...
      }
   }
   a /= float((W - 1) * H);
   //Calcualate the mean of the gradient in ih-direction
   for (int ih = 0; ih < H; ih++) {
      for (int iw = 0; iw < W - 1; iw++) {
         b += wrapToPi(t->get_raw_value_at(iw, ih + 1) - t->get_raw_value_at(iw, ih));
      }
   }
   b /= float(W * (H - 1));

   //TODO: Improve Offset:
   c = t->get_raw_value_at(0, 0);
   float phi_f = 0.f;

   for (int ih = 0; ih < H; ih++) {
      for (int iw = 0; iw < W - 1; iw++) {
         phi_f = a * iw + b * ih + c;
         t->set_value_at(iw, ih, phi_f + wrapToPi(t->get_value_at(iw, ih) - phi_f));
         /* Set Value decreases the value by the tile offset */
      }
   }
}

std::string grad_fit_tile_unwrapper::get_name() {
   std::string name = "grad";
   return name;
}

void grad_fit_tile_unwrapper::usage_help() {
   PRINTLN("*------------------------------------------------------------*");
   PRINTLN("Usage of the grad fit tile uwnrapper...");
   PRINTLN("No Options. DO NOT USE this unwrapper: DEPRECATED");
   PRINTLN("*------------------------------------------------------------*");
}