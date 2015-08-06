/* 
 * File:   model_gradient.h
 * Author: b.steltner
 *
 * Created on 17. Dezember 2014, 12:31
 */

#ifndef MODEL_GRADIENT_H
#define	MODEL_GRADIENT_H

#include "model/abstract_gradient.h"


//FORWARD DECLARATION START
class abstract_function; 
//FORWARD DECLARATION END

class model_gradient : public abstract_gradient {
public:
    
    model_gradient(sharedptr<std::vector<abstract_function> > vc_base_functions);
    virtual ~model_gradient();

    /**
     * returns the value of the function at (x,y)
     * @param x coordinate
     * @param y coordinate
     * @return value of the gradient function in x and y 
     */
    sharedarray<float> eval(float x, float y);

private:
    //! Hidden std constructor
    model_gradient();
    sharedptr<std::vector<sharedptr<abstract_gradient> > > vc_base_gradients;
    sharedptr<std::vector<float> > vc_coeff;
};

#endif	/* MODEL_GRADIENT_H */

