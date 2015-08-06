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
 * File:   in_out.h
 * Author: g.antonopoulos
 * 
 * Created on 15. April 2014, 08:49
 */

#ifndef IN_OUT_H
#define	IN_OUT_H

#include "basic_includes.h"
#include "image/row_major_float_image.h"
#include <cstdio> //Muss hier sein wg der template function

/**
 * Enthält Funktionen zum Input und Output.
 */



/**
 * Fill a float image with data from a file. File must be in binary format with 32bit float values.
 * @param inputfile The input file
 * @param img The float_image
 * @return true if successful, false otherwise.
 */
bool read_float_image(char *inputfile, float_image * img);


/**
 * Fill a float image with data from a file. File must be in binary format with values
 * corresponding to the given template type. The data is then read in the format
 * specified by the template parameter and casted to float values.
 * @param inputfile The input file
 * @param img The float_image
 * @return true if successful, false otherwise.
 */
template<class T> bool read_image(char *inputfile, float_image * img) //Implementation muss im Header passieren wg template.
{
    int imsize = img->get_width()*img->get_height();
    T * bild = new T[imsize];
    
    DEBUG_PRINTLN("Size of image primitive = "<< sizeof(T)*8 << "bit");
    
    FILE *pFile;
    pFile = fopen(inputfile,"rb");
    if(pFile == NULL)
    {
        return false;
    }
    else
    {
        size_t len = fread(bild,sizeof(T),imsize,pFile);
        fclose(pFile);
        
        if( len == 0 || ferror(pFile) || feof(pFile))
        {
            return false;
        }
    }
    
    //Jetzt das bild elementweise in floats casten
    float * imgdata = img->get_data_pointer();
    for(int i = 0; i < imsize; i++)
    {
        imgdata[i] = static_cast<float>(bild[i]);
    }
    delete[] bild;
    return true;
}

/**
 * Fill a float image with data from a file. File must be in binary format with values
 * corresponding to the given template type. The data is then read in the format
 * specified by the template parameter and casted to float values.
 * @param inputfile The input file
 * @param img The float_image
 * @return true if successful, false otherwise.
 */
template<class T> bool read_image(char *inputfile, sharedptr<row_major_float_image> img) //Implementation muss im Header passieren wg template.
{
    int imsize = img->get_width()*img->get_height();
    T * bild = new T[imsize];
    
    DEBUG_PRINTLN("Size of image primitive = "<< sizeof(T)*8 << "bit");
    
    FILE *pFile;
    pFile = fopen(inputfile,"rb");
    if(pFile == NULL)
    {
        DEBUG_PRINTLN("Error in in_out::read_image: Failed to open image (a)");
        return false;
    }
    else
    {
        size_t len = fread(bild,sizeof(T),imsize,pFile);
        fclose(pFile);
        
        if( len == 0 || ferror(pFile) || feof(pFile))
        {
            DEBUG_PRINTLN("Error in in_out::read_image: Input length: "<<len);
            return false;
        }
    }
    
    //Jetzt das bild elementweise in floats casten
    float * imgdata = img->get_data_pointer();
    for(int i = 0; i < imsize; i++)
    {
        imgdata[i] = static_cast<float>(bild[i]);
    }
    delete[] bild;
    return true;
}

/**
 * Write contents of a float_image to binary file.
 * @param outputfile The output file.
 * @param img The float image.
 * @return True if successful, false otherwise.
 */
bool write_image(char *outputfile, float_image * img);


/**
 * Write contents of a float_image to binary file.
 * @param outputfile The output file.
 * @param img The float image.
 * @return True if successful, false otherwise.
 */
bool write_image(char *outputfile, sharedptr<float_image> img);

/**
 * Write contents of a float_image to binary file.
 * @param outputfile The output file.
 * @param img The float image.
 * @return True if successful, false otherwise.
 */
bool write_image(char *outputfile, sharedptr<row_major_float_image> img);



#endif	/* IN_OUT_H */

