/* 
 * File:   monomial_gradient.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 10. Dezember 2014, 15:58
 */

#include "model/monomial_gradient.h" 
#include "debug/debug_out.h"

monomial_gradient::monomial_gradient(int m, int n) {
   this->x_exp = m;
   this->y_exp = n;
}

monomial_gradient::~monomial_gradient() {
   //empty
}

sharedarray<float> monomial_gradient::eval(float x, float y) {
   
   float * grad_val = new float[2]{0.0f, 0.0f};
   
   if (this->x_exp > 0 && this->y_exp > 0) 
   { // at least x*y 
      grad_val[0] = this->x_exp * std::pow(x, this->x_exp - 1) * std::pow(y, this->y_exp); //<=> 
      grad_val[1] = std::pow(x, this->x_exp) * this->y_exp * std::pow(y, this->y_exp - 1); //<=> 
   }
   else if (this->x_exp == 0 && this->y_exp == 0)
   { //x^0y^0 => nothing to derive here
      grad_val[0] = 0.f;
      grad_val[1] = 0.f;
   }
   else if (this->x_exp > 0)
   { //x^n * y^0 => x^n 
      grad_val[0] = this->x_exp * std::pow(x, this->x_exp - 1); //y^0 => 1
      grad_val[1] = 0.f;
   }
   else if (this->y_exp > 0)
   { //x^0 * y^m => y^m 
      grad_val[0] = 0.f;
      grad_val[1] = this->y_exp * std::pow(y, this->y_exp - 1);
   }
   else
   {
       //there is no other case :)
       PRINTLN("The impossible has happened in " << __FILE__ << " in " << __LINE__);
   }
   
   sharedarray<float> gradient(grad_val);
   return gradient;
}