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
 * File:   image.h
 * Author: g.antonopoulos
 *
 * Created on 1. April 2014, 12:09
 */

#ifndef FLOAT_IMAGE_H
#define	FLOAT_IMAGE_H

/**
 * @class float_image Diese Klasse speichert ein 2D Bild, welches in einem 1D
 * Datenvektor gespeichert wird. Die Ordnung des
 * Datenvektors gibt die Klasse nicht vor. Es müssen auch nicht alle Daten
 * hintereinander weg liegen. Die Zugriffsoperatoren sollen überladen werden,
 * um eine bestimmte Art der Speicherung (z.b. row-major / column-major) oder
 * sonstwas zu abstrahieren.
 * Wichtig: Beim Aufruf des Destruktors des Bildes wird das entsprechende Array
 * nicht freigegeben!
 */
class float_image {
private:

    /**
     * The = operator should not be used to prevent misunderstandings. The code
     * float_image f(640,480);
     * f = my_float_image;
     * could result in memory leak since f allocates memory for its data array
     * at creation that is not deallocated. Also f and my_float_image must have
     * same size to produce meaningful code...
     * @param f
     * @return 
     */
    float_image & operator=(const float_image & f) {
        this->width = f.width;
        this->height = f.height;
        this->data = f.data;
        return *this;
    }

protected:
    long width; //first dimension
    long height; //second dimenstion
    float * data;


public:

    /**
     * Create float image with and allocate data array! The data will not be zero
     * filled.
     * @param width
     * @param height
     */
    float_image(long width, long height);

    /**
     * Create float image with specified data array. The image will operate on this
     * array.
     * @param data
     * @param width
     * @param height
     */
    float_image(float* data, long width, long height);



    virtual ~float_image();

    /**
     * @return image width
     */
    virtual long get_width() const;

    /**
     * @return image height
     */
    virtual long get_height() const;

    /**
     * 
     * @return pointer to data array
     */
    virtual float * get_data_pointer();
    /**
     * Return element at position width = w, height = h, starting with (0,0)
     * in upper left corner of the image. Implemented in child classes, see e.g.
     * row_major_float_image.
     */
    virtual float & operator()(long w, long h) = 0;

    virtual float operator()(long w, long h) const = 0;

    /**
     * Fill image with zeroes.
     */
    virtual void zero_fill();

    /**
     * Copies the data from this image into the given image. Both images need to have the
     * same dimensions.
     * @param img Data from this image is copied into img.
     * @return true if successful, false if not.
     */
    virtual bool copy_data_to(float_image * img);


    /**
     * Free memory associated with the image. This is not done when the
     * destructor is called.
     */
    virtual void clear_mem();

    /**
     * Get Pixel value at specified position.
     * @param iw
     * @param ih
     * @return 
     */
    virtual float get_pixel(long iw, long ih);

    /**
     * Set Pixel value at specified position.
     * @param iw
     * @param ih
     * @param val
     * @return 
     */
    virtual void set_pixel(long iw, long ih, float val);
};

/**
 * This method provides a deleter for a float image. It makes image free
 * the allocated memory and deletes the pointer. It is mainly used for providing
 * a custom deleter for boost:shared_ptr
 * @param img
 */
void delete_float_image(float_image * img);


#endif	/* IMAGE_H */

