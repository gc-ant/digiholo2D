/* 
 * File:   least_squares_grad_unwrapper.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 16. Dezember 2014, 11:29
 */

#include "algorithm/least_squares_grad_unwrapper.h"
#include <fstream>
#include <exception>

#include "algorithm/second_order_gradient.h"
#include "algorithm/strand_tile_unwrapper.h"
#include "model/model_function.h"
#include "model/monomial_function.h"
#include "tiles/unit_cartesian_coordinate_system.h"


#include "debug/debug_out.h"
#include "debug/debug_time.h"

least_squares_grad_unwrapper::least_squares_grad_unwrapper()
{
   sharedptr<unit_cartesian_coordinate_system> coord_system(new unit_cartesian_coordinate_system());
   this->grad_calculator.reset(new second_order_gradient(coord_system));
}

least_squares_grad_unwrapper::least_squares_grad_unwrapper(std::vector<std::string> usettings) {
   this->init(usettings);
   sharedptr<unit_cartesian_coordinate_system> coord_system = boost::dynamic_pointer_cast<unit_cartesian_coordinate_system, abstract_coordinate_system>(model->get_coordinate_system());
   this->grad_calculator.reset(new second_order_gradient(coord_system));
}

least_squares_grad_unwrapper::least_squares_grad_unwrapper(sharedptr<model_function> model) {
   this->model = model;
}

least_squares_grad_unwrapper::~least_squares_grad_unwrapper() {

}

void least_squares_grad_unwrapper::unwrap(boost::shared_ptr<tile> t) {
   //   debug_time * time = new debug_time(); 
   //1) Calculate Matrix A (if necessary)
   if (!A) { // if the pointer to A is empty, it must be recalculated
      this->calc_A_and_SVD(t);
   } else if ((A->rows() != 2 * t->get_height() * t->get_width()) || (A->cols() != this->model->get_base_size())) { // if the number of rows is not 2*WIDTH*HEIGHT or the number of base functions is != number of cols, the matrix has to be recalculated
      DEBUG_PRINTLN("----- Recalculation of design matrix A necessary. This takes additional time and slows down the algorithm. Prevent it by choosing the tilecount as a factor of the dimension -----");
      this->calc_A_and_SVD(t);
   } else {  // no recalculation necessary. Number of elements or base functions has not changed since last unwrapped tile.
   }

   //2) Calculate vector b
   sharedptr<Eigen::VectorXf> b = calc_and_get_b(t);

   //3) Calculate the coefficients for the least squares solution for the coefficients of the model function
   sharedptr<Eigen::VectorXf> coeff(new Eigen::VectorXf());
   (*coeff) = this->svd_of_A->solve(*b);
   this->model->set_coeff_vector(coeff);

   /*
      4) Unwrap the tile using formula phi_un = W(phi_w - f_model) + f_model
      todo better idea for constant offset??
      todo: BESSER NOCH. Eigene Funktion zum minimieren von Varianz von W(T-const)+const schreiben. Dann konst als Offset in die Funktion oder so.
    */
    
   //With Strand Unwrapper
   this->model->subtract_wrap_add_function(t, this->strand_steps);
   
   //Alternatively @todo this is faster, but less robust.
   //this->model->set_constant_offset(t->get_value_at(0,0));
   //this->model->subtract_wrap_add_function(t, 0);
   
   /**
    *   sharedptr<abstr....unwrapper> stu
    *  this->model->subtract_wrap_add_function(t, sharedptr<abstract_tile_unwrapper> strand_bla);
    */
   //   DEBUG_PRINTLN("Unwrap took: ");
   //   delete time; 
}

void least_squares_grad_unwrapper::calc_A_and_SVD(sharedptr<tile> t) {
   debug_time * time = new debug_time();
   /* Design decision for A: Aij = fj(xi).
    * Notation: f1x(0,0) is the first base_function gradient in x direction at the point 0,0.
    * f4y(1,2) is therefore the forth bf gradient in y direction at the point w=1,h=2.
    * Matrix A: 
    * |f1x(0,0)     ...    fmx(0,0)|
    * |f1x(1,0)     ...    fmx(1,0)|
    * |f1x(2,0)     ...    fmx(2,0)|
    * | ...         ...      ...   |
    * |f1x(W-1,0)   ...  fmx(W-1,0)|
    * |f1x(0,1)     ...    fmx(0,1)|
    * | ...         ...      ...   |
    * |f1x(W-1,H-1) ...fmx(W-1,H-1)|
    * |f1y(0,0)     ...    fmy(0,0)|
    * |f1y(1,0)     ...    fmy(1,0)|
    * | ...         ...      ...   |
    * |f1y(W-1,H-1) ...fmy(W-1,H-1)|
    */
   long W = t->get_width();
   long H = t->get_height();
   sharedptr<std::vector<sharedptr<abstract_function> > > vc_base = this->model->get_base_vector();

   A.reset(new Eigen::MatrixXf(2 * W*H, vc_base->size()));

   /* START - Prepare for-loop */
   /* Declare pure temporary objects */
   sharedptr<abstract_gradient> base_gradient;
   sharedarray<float> gradient;
   std::vector<float> grad_x, grad_y;
   grad_x.reserve(W * H);
   grad_y.reserve(W * H);
   float x, y;
   sharedptr<unit_cartesian_coordinate_system> coordsys = boost::dynamic_pointer_cast<unit_cartesian_coordinate_system, abstract_coordinate_system>(model->get_coordinate_system());
   coordsys->init(t);
   /* END - Preparation */

   //1) Calculate the Matrix A and store it in the member variable

   /* Iterate over every base_function <=> column of the matrix */



   for (unsigned int j = 0; j < vc_base->size(); j++) {

      base_gradient = vc_base->at(j)->get_gradient();
      //      sharedptr<monomial_gradient> monom = boost::static_pointer_cast<monomial_gradient>(base_gradient); //EXCLUDE
      //      monom->testgradient(2, 2);  //EXCLUDE 

      /* Iterate from "left to right" of the tile, afterwards go one "down" and repeat from "left to right" */
      for (int iy = 0; iy < H; iy++) { //Column
         for (int ix = 0; ix < W; ix++) { //Row
            x = coordsys->calc_x_at(ix, iy);
            y = coordsys->calc_y_at(ix, iy);
            gradient = base_gradient->eval(x, y); //Returns {gradx_float, grady_float}
            grad_x.push_back(gradient[0]); //Save gradx into vector
            grad_y.push_back(gradient[1]); //Save grady into vector
         }
      }

      /* Insert the x-grad-vector into the matrix. For first base_function it should reach until W*H-1 index */
      for (int i = 0; i < W * H; i++) {
         (*A)(i, j) = grad_x.at(i);
      }
      /* Insert the y-grad-vector into the matrix. Above example, should reach from W*H to 2W*H-1 index*/
      for (int i = 0; i < W * H; i++) {
         (*A)(W * H + i, j) = grad_y.at(i);
      }

      grad_x.clear();
      grad_y.clear();
   }

   //2) Calculate the SVD of the matrix A and store it in the member variable
   this->svd_of_A.reset(new Eigen::JacobiSVD<Eigen::MatrixXf> (*A, Eigen::ComputeThinU | Eigen::ComputeThinV));
   
   time->get_time();
   DEBUG_PRINTLN("Calculating A took: ");
   delete time;
}

sharedptr<Eigen::VectorXf> least_squares_grad_unwrapper::calc_and_get_b(sharedptr<tile> t) {
   //   debug_time * time = new debug_time();
   second_order_gradient sec_o_grad(this->model->get_coordinate_system());
   //   DEBUG_PRINTLN("Calculating b took:" ); 
   //   delete time; 
   return sec_o_grad.get_gradient_eigen_vector(t);
}

void least_squares_grad_unwrapper::init(std::vector<std::string> usettings) {
   if (usettings.empty()) {
      DEBUG_PRINTLN("Error: MLSQU unwrapper needs a model input...")
      usage_help();
      exit(0);
   }
   this->strand_steps = 0;
   std::vector<std::string>::iterator it;
   for (it = usettings.begin(); it != usettings.end(); ++it) {
      std::string name = it->substr(0, it->find_first_of('-'));
      std::string options = it->substr(it->find_first_of('-') + 1);

      if (name.compare("polynom") == 0) {
         sharedptr<abstract_coordinate_system> coord_system(new unit_cartesian_coordinate_system());
         /* Options should look like: 2-3 or 14-16 */
         int max_x_exp = boost::lexical_cast<int>(options.substr(0, options.find_first_of('-')));
         int max_y_exp = boost::lexical_cast<int>(options.substr(options.find_first_of('-') + 1, options.length()));

         sharedptr<std::vector<sharedptr<abstract_function> > > vc_base(new std::vector<sharedptr<abstract_function> >);
         for (int x_exp = 0; x_exp <= max_x_exp; x_exp++) {
            for (int y_exp = 0; y_exp <= max_y_exp; y_exp++) {
               if (x_exp == 0 && y_exp == 0) {
                  y_exp++;
               }
               sharedptr<abstract_function> temp(new monomial_function(x_exp, y_exp));
               vc_base->push_back(temp);
            }
         }
         this->options_name = "poly-" + boost::lexical_cast<std::string>(max_x_exp) + "-" + boost::lexical_cast<std::string>(max_y_exp);
         sharedptr<model_function> my_model(new model_function(vc_base, coord_system));
         this->model = my_model;
      } else if (name.compare("steps") == 0) {
         if (!options.empty()) {
            this->strand_steps = boost::lexical_cast<int>(options);
            DEBUG_PRINTLN("Using " << this->strand_steps << " steps for evaluating best offset");
         } else {
            this->strand_steps = 10;
            DEBUG_PRINTLN("-v steps-x needs an integer x - otherwise set to " << this->strand_steps << " (default)");
         }
      } else {
         DEBUG_PRINTLN("")
         DEBUG_PRINTLN("Unknown option for MLSQU unwrapper: " << name);
         usage_help();
         exit(0);
      }
   }
   if(this->strand_steps == 0)  this->strand_steps = 10;           
}

std::string least_squares_grad_unwrapper::get_name() {
   std::string name = "mlsqu_";
   name += this->options_name;
   return name;
}

void least_squares_grad_unwrapper::usage_help() {
   DEBUG_PRINTLN("*--------------------------------------------------------------------------*");
   DEBUG_PRINTLN("Usage of the MLSQU uwnrapper...");
   DEBUG_PRINTLN("Option(s)");
   DEBUG_PRINTLN("polynom-i-j :  Design polynom of leading order x^i * y^j.");
   DEBUG_PRINTLN("               Eg. polynom-3-3 leads to the following polynom: ");
   DEBUG_PRINTLN("               f(x) = const + x + y + xy + x^2 + ... + x^2y^2 + ... + x^3y^3");
   DEBUG_PRINTLN("steps-x     :  Number of steps for the integrated strand unwrapper");
   DEBUG_PRINTLN("*--------------------------------------------------------------------------*");
}