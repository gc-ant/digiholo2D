#include "image/row_major_float_image.h"


float & row_major_float_image::operator() (long w, long h)
{
    return this->data[h*width+w];
}

float row_major_float_image::operator() (long w, long h) const
{
    return this->data[h*width+w];
}

boost::shared_ptr<row_major_float_image> create_row_major_float_image(long width, long height)
{
    boost::shared_ptr<row_major_float_image> p(new row_major_float_image(width, height),delete_float_image);
    return p;
}

boost::shared_ptr<row_major_float_image> create_row_major_float_image(long width, long height, float * data, bool owns_data)
{
    if(owns_data == true)
    {
        //with custom deleter that frees the pointer and the data memory of the image
        boost::shared_ptr<row_major_float_image> p(new row_major_float_image(width, height),delete_float_image);
        return p;
    }
    else
    {
        //only frees the pointer.
        boost::shared_ptr<row_major_float_image> p(new row_major_float_image(width, height));
        return p;
    }
}