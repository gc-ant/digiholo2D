/*
* Copyright 2015 G.C. Antonopoulos and B. Steltner, Laser Zentrum Hannover e.V.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* 
 * File:   col_major_image.h
 * Author: g.antonopoulos
 *
 * Created on 14. Mai 2014, 13:46
 */

#ifndef COL_MAJOR_IMAGE_H
#define	COL_MAJOR_IMAGE_H

#include "image/float_image.h"

class col_major_float_image : public float_image
{
public: 
    
    
    col_major_float_image(float* data, long width, long height):float_image(data,width, height)
    {

    }
    
    /**
     * Generate image that reserves memory
     * @param width
     * @param height
     */
    col_major_float_image(long width, long height):float_image(width, height)
    {

    }
    
    virtual float & operator() (long w, long h)
    {
        return this->data[w*height+h];
    }
    
    virtual float operator() (long w, long h) const
    {
        return this->data[w*height+h];
    }
};


#endif	/* COL_MAJOR_IMAGE_H */

