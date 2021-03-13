#include "image/in_out.h"
#include <cstdio>
#include "debug/debug_out.h"


bool read_float_image(char *inputfile, float_image * img)
{
    int imsize = img->get_width()*img->get_height();
    
    FILE *pFile;
    pFile = fopen(inputfile,"rb");
    if(pFile == NULL)
    {
        return false;
    }
    else
    {
        size_t len = fread(img->get_data_pointer(),sizeof(float),imsize,pFile);
        fclose(pFile);
        
        if( len == 0 || ferror(pFile) || feof(pFile))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}


bool write_image(std::string outputfile, float_image * img)
{
    long imsize = img->get_width()*img->get_height();
    
    PRINTLN("Writing image to disk (" << outputfile << ")");
    
    FILE *pFile;
    pFile = fopen(outputfile.c_str(),"wb");
    if(pFile == NULL)
    {
        return false;
    }
    else 
    {
        fwrite(img->get_data_pointer(),sizeof(float),imsize,pFile);
        fclose(pFile);
        
        if(ferror(pFile))
        {
            PRINTLN("Error writing image to disk!");
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool write_image(std::string outputfile, sharedptr<row_major_float_image> img)
{
    long imsize = img->get_width()*img->get_height();

    FILE *pFile;
    pFile = fopen(outputfile.c_str(),"wb");
    if(pFile == NULL)
    {
        return false;
    }
    else
    {
        fwrite(img->get_data_pointer(),sizeof(float),imsize,pFile);
        fclose(pFile);
        
        if(ferror(pFile))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}