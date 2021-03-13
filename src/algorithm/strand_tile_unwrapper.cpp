/* 
 * File:   strand_tile_unwrapper.cpp
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 25. November 2014, 11:48
 */

#include "algorithm/strand_tile_unwrapper.h"
#include <limits> //to access float::max
#include <stdio.h>

strand_tile_unwrapper::strand_tile_unwrapper(int N_rho) {
    this->N_rho = N_rho;
}

strand_tile_unwrapper::strand_tile_unwrapper(std::vector<std::string> usettings) {
    this->N_rho = 5; 
    if (usettings.size() != 0) {
        std::string option, value;
        std::vector<std::string>::iterator it;

        for (it = usettings.begin(); it != usettings.end(); ++it) {
            option = it->substr(0, it->find_first_of("-"));
            value = it->substr(it->find_first_of("-") + 1);
            if (option.compare("steps") == 0) {
                this->N_rho = boost::lexical_cast<int>(value);
            } else if (option.compare("help") == 0) {
                this->usage_help();
            } else {
                PRINTLN("Option \"" << option << "\" not supported. Please use steps-5 (value arbitrary).");
            }
        }
    }
}

strand_tile_unwrapper::~strand_tile_unwrapper() {

}

void strand_tile_unwrapper::unwrap(boost::shared_ptr<tile> t) {
    float delta_rho = 2.f * M_PI / (this->N_rho);

    float min_rho_pos = 0.0f;
    float temp_norm = FLT_MAX;
    const float EPSILON = 0.01;
    float min_val = abs_diff_norm(t, temp_norm);
    
    
    //Erhöhe cur_rho um delta_rho Schritte von ~0 bis 2Pi
    for (int i = 1; i <= this->N_rho; i++)
    {
        t->rewrap((float)(i) * delta_rho);
        temp_norm = abs_diff_norm(t, min_val);
        
        if (temp_norm < min_val) 
        { //Wenn aktueller Wert besser: Setze Minimum-Variablen
            min_val = temp_norm;
            min_rho_pos = float(i) * delta_rho;
            
            //!@todo remove this hack. Problem with non-constant runtime (?)
            //@todo wieder rein? THIS CAN LEAD TO NON CONSTANT RUNTIME!!! 
//            if (temp_norm * temp_norm < EPSILON)
//            { //Best value, no point in doing further. //TODO: eps * eps * 1.000.000 best value? 
//                //end this method...                    
//                return;
//            }
        }
        t->rewrap(-float(i) * delta_rho);
    }
    t->rewrap(min_rho_pos);
    //    PRINTLN(min_rho_pos/delta_rho);
    //    PRINTLN("");
}

float strand_tile_unwrapper::abs_diff_norm(boost::shared_ptr<tile> t, float cur_min_val) {
    float sum_w = 0.f;

    long width = t->get_width();
    float one_over_w = 1.f / ((float) width);
    long height = t->get_height();
    float one_over_h = 1.f / ((float) height);

    for (long ih = 0; ih < height; ih++) {
        for (long iw = 0; iw < width - 1; iw++) {
            sum_w += abs(t->get_value_at(iw, ih) - t->get_value_at(iw + 1, ih));
        }
        //@todo wieder rein? THIS WILL ALSO LEAD TO NON CONSTANT RUNTIME, WTF BENJAMIN!
//        if (cur_min_val < one_over_w * sum_w) { //Lokale Abbruchbedingung: Ausdruck ist größer als bisher kleinster Wert
//            return one_over_w * sum_w; //Breche Berechung ab
//        }
    }

    float sum_h = 0.f;

    for (long ih = 0; ih < height - 1; ih++){
        for (long iw = 0; iw < width; iw++) {
            sum_h += abs(t->get_value_at(iw, ih) - t->get_value_at(iw, ih + 1));
        }
        //@todo wieder rein? THIS WILL ALSO LEAD TO NON CONSTANT RUNTIME, WTF BENJAMIN!
//        if (cur_min_val < one_over_w * sum_w + one_over_h * sum_h) { //Lokale Abbruchbedingung: Ausdruck ist größer als bisher kleinster Wert
//            return one_over_w * sum_w + one_over_h*sum_h; //Breche Berechung ab
//        }
    }
    return one_over_w * sum_w + one_over_h*sum_h;
}

std::string strand_tile_unwrapper::get_name() {
    std::string name = "strand_n";
    name += boost::lexical_cast<std::string>(this->N_rho);
    return name;
}

void strand_tile_unwrapper::usage_help() {
    PRINTLN("*------------------------------------------------------------*");
    PRINTLN("Usage of the strand  uwnrapper...");
    PRINTLN("Option(s)");
    PRINTLN("steps-x :  Method will test x values for getting the best offset");
    PRINTLN("*------------------------------------------------------------*");
}