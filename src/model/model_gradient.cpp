/* 
 * File:   model_gradient.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 17. Dezember 2014, 12:31
 */

#include "model/model_gradient.h"

#include "model/abstract_function.h"

model_gradient::model_gradient() {
    //empty
}

model_gradient::model_gradient(sharedptr<std::vector<abstract_function> > vc_base_functions) {
    std::vector<abstract_function>::iterator it;
    sharedptr<std::vector<sharedptr<abstract_gradient> > >gradients(new std::vector<sharedptr<abstract_gradient> >);

    for (it = vc_base_functions->begin(); it != vc_base_functions->end(); ++it) {
        gradients->push_back(it->get_gradient());
    }
    this->vc_base_gradients = gradients;
    sharedptr<std::vector<float> > coeff(new std::vector<float>(gradients->size(), 1.f));
    this->vc_coeff = coeff;
}

model_gradient::~model_gradient() {

}

sharedarray<float> model_gradient::eval(float x, float y) {
    sharedarray<float> value(new float[2]);
    //   sharedarray<float> value (new float[2]);
    if (this->vc_base_gradients->size() != this->vc_coeff->size()) {
        DEBUG_PRINTLN("Error in model_gradient::eval(x, y, vc_coeff): Amount of base gradients unequal to size of coefficents!");
        return value;
    }
    std::vector<sharedptr<abstract_gradient> >::iterator it_base = this->vc_base_gradients->begin();
    std::vector<float>::iterator it_coeff = vc_coeff->begin();

    float grad_x = 0.f;
    float grad_y = 0.f;

    while (it_base != this->vc_base_gradients->end()) { //Iterate over base or coeff doesnt matter, both same size.
        value = (*it_base)->eval(x, y);
        grad_x += (*it_coeff) * value[0];
        grad_y += (*it_coeff) * value[1];
        ++it_base;
        ++it_coeff;
    }
    value[0] = grad_x;
    value[1] = grad_y;
    return value;
}


