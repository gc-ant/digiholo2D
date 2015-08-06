/* 
 * File:   monomial_function.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 15:16
 */

#include "model/monomial_function.h"

#include "model/monomial_gradient.h"

monomial_function::monomial_function() {

}

monomial_function::~monomial_function() {

}

monomial_function::monomial_function(int m, int n) {
   this->x_exp = m;
   this->y_exp = n;
}

float monomial_function::eval(float x, float y) {
   //   float value = 0.f; 
   //   if(this->x_exp != 0) value += std::pow(x, this->x_exp);
   //   if(this->y_exp != 0) value += std::pow(y, this->y_exp);
   return std::pow(x, this->x_exp) * std::pow(y, this->y_exp);
}

sharedptr<abstract_gradient> monomial_function::get_gradient() {
   if (!this->gradient) {
      sharedptr<abstract_gradient> grad (new monomial_gradient(this->x_exp, this->y_exp));
      this->gradient = grad; 
   }
   return this->gradient;
}

int monomial_function::get_x_exp() {
   return this->x_exp;
}

int monomial_function::get_y_exp() {
   return this->y_exp;
}

