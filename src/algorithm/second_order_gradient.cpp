/* 
 * File:   second_order_gradient.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 16. Dezember 2014, 11:04
 */

#include "algorithm/second_order_gradient.h"
#include "tiles/unit_cartesian_coordinate_system.h"
#include "algorithm/wrap.h"

second_order_gradient::second_order_gradient(sharedptr<abstract_coordinate_system> coord_sys) {
   this->coordinate_system = coord_sys;
}

second_order_gradient::second_order_gradient() {
   this->coordinate_system = sharedptr<abstract_coordinate_system>(new unit_cartesian_coordinate_system());
}

second_order_gradient::~second_order_gradient() {
   //empty
}

// DUPLICATE METHOD OF get_gradient_eigen_vector. CHANGES HAS TO BE DONE TO BOTH METHODS!
sharedptr<std::vector<float> > second_order_gradient::get_gradient_floats(sharedptr<tile> t) {
   sharedptr<unit_cartesian_coordinate_system> coord_sys = boost::dynamic_pointer_cast<unit_cartesian_coordinate_system, abstract_coordinate_system>(this->coordinate_system);
   coord_sys->init(t);

   long W = t->get_width();
   float InvW = 1.f / (coord_sys->get_delta_x());
   long H = t->get_height();
   float InvH = 1.f / (coord_sys->get_delta_y());

   sharedptr<std::vector<float> > gradient(new std::vector<float>(2 * W * H));

   float value = 0.0f;
   /* We are saving row-major */
   /* Row-wise calculation of gradient in x and saving it to the gradient vector */

   long w = 0;
   long h = 0; //Indices

   /* Note: The gradient will be scaled with the amount of tiles in the direction */
   for (h = 0; h < H; h++) {
      value = InvW * wrapToPi(t->get_raw_value_at(1, h) - t->get_raw_value_at(0, h)); //Tile 0,h //Left Edge
      (*gradient)[h * W + w] = value;
      for (w = 1; w < W - 1; w++) {
         value = wrapToPi(t->get_raw_value_at(w + 1, h) - t->get_raw_value_at(w, h)); //Tile w,h   
         value += wrapToPi(t->get_raw_value_at(w, h) - t->get_raw_value_at(w - 1, h)); //Tile w,h   
         value *= .5f * InvW;
         (*gradient)[h * W + w] = value;
      }
      value = InvW * wrapToPi(t->get_raw_value_at(W - 1, h) - t->get_raw_value_at(W - 2, h)); //Tile W-1, h //Right Edge
      (*gradient)[h * W + w] = value;
   }

   /* Row-wise calculation of gradient in y and append it to the gradient vector starting at position WIDTH*HEIGHT */
   h = 0;
   for (w = 0; w < W; w++) {
      value = InvH * wrapToPi(t->get_raw_value_at(w, 1) - t->get_raw_value_at(w, 0)); //Tile: w,0
      (*gradient)[W * H + h * W + w] = value;
   }

   for (h = 1; h < H - 1; h++) {
      for (w = 0; w < W; w++) {
         value = wrapToPi(t->get_raw_value_at(w, h + 1) - t->get_raw_value_at(w, h));
         value += wrapToPi(t->get_raw_value_at(w, h) - t->get_raw_value_at(w, h - 1));
         value *= .5f * InvH;
         (*gradient)[W * H + h * W + w] = value;
      }
   }

   h = H - 1;
   for (w = 0; w < W; w++) {
      value = InvH * wrapToPi(t->get_raw_value_at(w, H - 1) - t->get_raw_value_at(w, H - 2));
      (*gradient)[W * H + h * W + w] = value;
   }

   return gradient;
}

// DUPLICATE METHOD OF get_gradient_floats. CHANGES HAS TO BE DONE TO BOTH METHODS!
sharedptr<Eigen::VectorXf> second_order_gradient::get_gradient_eigen_vector(sharedptr<tile> t) {
   sharedptr<unit_cartesian_coordinate_system> coord_sys = boost::dynamic_pointer_cast<unit_cartesian_coordinate_system, abstract_coordinate_system>(this->coordinate_system);
   coord_sys->init(t);

   long W = t->get_width();
   float InvW = 1.f / (coord_sys->get_delta_x());
   long H = t->get_height();
   float InvH = 1.f / (coord_sys->get_delta_y());

   sharedptr<Eigen::VectorXf> gradient (new Eigen::VectorXf(2*W*H));

   float value = 0.0f;
   /* We are saving row-major */
   /* Row-wise calculation of gradient in x and saving it to the gradient vector */

   long w = 0; //Indices
   long h = 0; 

   /* Note: The gradient will be scaled with the amount of tiles in the direction */
   for (h = 0; h < H; h++) {
      value = InvW * wrapToPi(t->get_raw_value_at(1, h) - t->get_raw_value_at(0, h)); //Tile 0,h //Left Edge
      (*gradient)[h * W + w] = value;
      for (w = 1; w < W - 1; w++) {
         value = wrapToPi(t->get_raw_value_at(w + 1, h) - t->get_raw_value_at(w, h)); //Tile w,h   
         value += wrapToPi(t->get_raw_value_at(w, h) - t->get_raw_value_at(w - 1, h)); //Tile w,h   
         value *= .5f * InvW;
         (*gradient)[h * W + w] = value;
      }
      value = InvW * wrapToPi(t->get_raw_value_at(W - 1, h) - t->get_raw_value_at(W - 2, h)); //Tile W-1, h //Right Edge
      (*gradient)[h * W + w] = value;
   }

   /* Row-wise calculation of gradient in y and append it to the gradient vector starting at position WIDTH*HEIGHT */
   h = 0;
   for (w = 0; w < W; w++) {
      value = InvH * wrapToPi(t->get_raw_value_at(w, 1) - t->get_raw_value_at(w, 0)); //Tile: w,0
      (*gradient)[W * H + h * W + w] = value;
   }

   for (h = 1; h < H - 1; h++) {
      for (w = 0; w < W; w++) {
         value = wrapToPi(t->get_raw_value_at(w, h + 1) - t->get_raw_value_at(w, h));
         value += wrapToPi(t->get_raw_value_at(w, h) - t->get_raw_value_at(w, h - 1));
         value *= .5f * InvH;
         (*gradient)[W * H + h * W + w] = value;
      }
   }

   h = H - 1;
   for (w = 0; w < W; w++) {
      value = InvH * wrapToPi(t->get_raw_value_at(w, H - 1) - t->get_raw_value_at(w, H - 2));
      (*gradient)[W * H + h * W + w] = value;
   }

   return gradient;
}


