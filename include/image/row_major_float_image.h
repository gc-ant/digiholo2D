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
 * File:   row_major_image.h
 * Author: g.antonopoulos
 *
 * Created on 9. April 2014, 11:17
 */

#ifndef ROW_MAJOR_IMAGE_H
#define	ROW_MAJOR_IMAGE_H

#include "basic_includes.h"
#include "image/float_image.h"

class row_major_float_image : public float_image
{
public: 
    
    
    row_major_float_image(float* data, long width, long height):float_image(data,width, height){}
    
    virtual ~row_major_float_image() {}
    
    /**
     * Generate image that reserves memory
     * @param width
     * @param height
     */
    row_major_float_image(long width, long height):float_image(width, height){}
    
    virtual float & operator() (long w, long h);
    virtual float operator() (long w, long h) const;
    
};

/**
 * Creator method that provides a strong pointer to a new row_major_float_image.
 * This pointer will free the memory occupied by the float image when there
 * are no more strong references to this float image.
 * @param width
 * @param height
 * @return 
 */
boost::shared_ptr<row_major_float_image> create_row_major_float_image(long width, long height);
/**
 * Creator method that provides a strong pointer to a new row_major_float_image.
 * If the owns_data argument is true, then the pointer will free the memory
 * occupied by the given data array once no more strong pointers to the
 * image exist. If the owns_data argument is false then the data array will
 * not be deleted on the destruction of the float_image. This may lead to memory
 * leaks and should be avoided at any cost.
 * @param width
 * @param height
 * @param data Must not be NULL. Use overloaded method for creation of empty image.
 * @param owns_data
 * @return 
 */
boost::shared_ptr<row_major_float_image> create_row_major_float_image(long width, long height, float * data, bool owns_data = true);



#endif	/* ROW_MAJOR_IMAGE_H */

