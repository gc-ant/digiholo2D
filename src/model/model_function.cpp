/* 
 * File:   model_function.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 15:07
 */

#include "model/model_function.h"

#include "tiles/tile.h"
#include "model/monomial_function.h"
#include "algorithm/strand_tile_unwrapper.h"
#include "tiles/unit_cartesian_coordinate_system.h"
#include "debug/debug_out.h"
#include "debug/debug_time.h"

/**
 * Helper method that will return a pointer to a new vector of floats
 * containing the same elements as the given Eigen VectorXf in the same order.
 * 
 * CAREFUL! Do not use this function as an unnamed variable in a function argument.
 * That is, do not write foo(to_Eigen_VectorXf(v1)) but v2 = to_Eigen_VectorXf(v1); foo(v2).
 * See the Boost Library Guide on shared_ptr.
 * 
 * @param v Pointer to the VectorXf instance
 * @return Pointer to the std:vector
 */
sharedptr<std::vector<float> > to_std_vector(sharedptr<Eigen::VectorXf> v);

model_function::model_function() {
   this->const_offset = 0.f;
   sharedptr<abstract_coordinate_system> coord_system(new unit_cartesian_coordinate_system());
   this->coordinate_system = coord_system;
}

model_function::model_function(sharedptr<std::vector<sharedptr<abstract_function> > > vc_base, sharedptr<abstract_coordinate_system> coord_system) :coordinate_system(coord_system), const_offset(0.f), vc_base_functions(vc_base)
{
//empty
}

model_function::~model_function() {
}

/**
 * Calculate the value of this function.
 * @param x coordinate
 * @param y coordinate
 * @param vc_coeff set of coefficients for the linear combination of the base functions. 
 * NOTE: vc_coeff must have the same amount of elements as there are base functions.
 * @return value of the function
 */
float model_function::eval(float x, float y, sharedptr<std::vector<float> >vc_coeff) {
   if (this->vc_base_functions->size() != vc_coeff->size()) {
      DEBUG_PRINTLN("Error in model_function::eval(x, y, vc_coeff): Amount of base functions unequal to size of coefficents!");
      return -1.f; 
   }
   float value = 0.0f;

   std::vector<sharedptr<abstract_function> >::iterator it_base = this->vc_base_functions->begin();
   std::vector<float>::iterator it_coeff = vc_coeff->begin();

   //Evaluate the base functions and add them according to the coefficients.
   while (it_base != this->vc_base_functions->end()) { //Iterate over base or coeff doesnt matter, both same size.
      value += (*it_coeff) * (*it_base)->eval(x, y); //Equivalent altogether to: Sum over every base of coefficient * value of base function at (x,y)
      ++it_base;
      ++it_coeff;
   }
   //Lastly, add the constant offset of this function, since it is treated separately
   value += this->const_offset;

   return value;
}

float model_function::eval(float x, float y, sharedptr<Eigen::VectorXf> vc_coeff) {
   sharedptr<std::vector<float> > pvec = to_std_vector(vc_coeff);
   return eval(x, y, pvec);
}

/**
 * Calculate the value of this function with "build-in" coefficients.
 * @param x coordinate
 * @param y coordinate
 * @return value of the function
 */
float model_function::eval(float x, float y) {
   return this->eval(x, y, this->vc_coeff);
}

void model_function::set_base_vector(sharedptr<std::vector<sharedptr<abstract_function> > > vc_base) {
   this->vc_base_functions = vc_base;
}

sharedptr<std::vector<sharedptr<abstract_function> > > model_function::get_base_vector() {
   return this->vc_base_functions;
}

void model_function::set_coeff_vector(sharedptr<std::vector<float> > vc_coeff) {
   this->vc_coeff = vc_coeff;
}

void model_function::set_coeff_vector(sharedptr<Eigen::VectorXf> vxf) {
   this->vc_coeff = to_std_vector(vxf);
}

int model_function::get_base_size() {
   return this->vc_base_functions->size();
}

int model_function::get_coeff_size() {
   return this->vc_coeff->size();
}

sharedptr<abstract_gradient> model_function::get_gradient() {
   return this->gradient;
}

sharedptr<abstract_coordinate_system> model_function::get_coordinate_system() {
   return this->coordinate_system;
}

void model_function::set_constant_offset(float value) {
   this->const_offset = value;
}

float model_function::get_constant_offset() {
   return this->const_offset;
}

void model_function::subtract_wrap_add_function(sharedptr<tile> t, int step_count) {
   long H = t->get_height();
   long W = t->get_width();

   sharedarray<float> func_values(new float [W * H]);

   for (int ih = 0; ih < H; ih++) {
      for (int iw = 0; iw < W; iw++) {
         func_values[ih * W + iw] = this->eval(this->coordinate_system->calc_x_at(iw, ih), this->coordinate_system->calc_y_at(iw, ih));
         t->add_value_at(iw, ih, -func_values[ih * W + iw]); //Note: Subtract value
      }
   }
   t->wrap();
   
   if(step_count != 0)
   {
      strand_tile_unwrapper stu(step_count);
      stu.unwrap(t);
   }

   for (int ih = 0; ih < H; ih++) {
      for (int iw = 0; iw < W; iw++) {
         t->add_value_at(iw, ih, func_values[ih * W + iw]); //Note: Add value
      }
   }
}



/////////////////////////////////////////////////////////////////////////////////

sharedptr<std::vector<float> > to_std_vector(sharedptr<Eigen::VectorXf> vecxf) {
   long N = vecxf->size(); //number of elements in vector
   sharedptr<std::vector<float> > stdvec(new std::vector<float>(N));

   for (int i = 0; i < N; i++) {
      (*stdvec)[i] = (*vecxf)(i);
   }

   return stdvec;
}
