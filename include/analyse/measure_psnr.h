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
 * File:   measure_psnr.h
 * Author: b.steltner
 *
 * Created on 8. Dezember 2014, 10:41
 */

#ifndef MEASURE_PSNR_H
#define	MEASURE_PSNR_H

#include "basic_includes.h"

//FORWARD DECLARATION START
class row_major_float_image; 
//FORWARD DECLARATION END


class measure_psnr {
public:
    /**
     * Constructor
     * @param noisy_img picture with noise
     * @param orig_img picture without noise
     * @param bit #bit the images have
     */
    measure_psnr(sharedptr<row_major_float_image> noisy_img, sharedptr<row_major_float_image> orig_img, int bit);
//    ~measure_psnr();
    /**
     * Calculates and returns the psnr of the given image
     * @return psnr
     */
    float calculate_psnr();
private:
    /**
     * Hidden std constructor
     */
    measure_psnr();
    sharedptr<row_major_float_image> noisy_img;
    sharedptr<row_major_float_image> orig_img;
    
    //# bits the image has
    int max; 

    /** 
     * This method calculates the difference of the the mean of the orignal image and the noisy image.
     * If there is only white noise, the offset should be a multiple of 2 pi, coming from raising values 
     * during the unwrapping.
     * Warning: Inconsistencies in the top left corner a criticial to this approach!
     * @return offset
     */
    float offset();
    
    /**
     * Calculate the mean of the block (0 - w,0 - h) of the image img
     * Warning: Inconsistencies in the top left corner a criticial to this approach!
     * @param w x-coordinate where to stop
     * @param h y-coordinate where to start
     * @param img image
     * @return mean of block (w,h)
     */
    float mean_of_block(long w, long h, sharedptr<row_major_float_image> img);
    
};

#endif	/* MEASURE_PSNR_H */

