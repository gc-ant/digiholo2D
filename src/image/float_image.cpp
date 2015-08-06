#include "image/float_image.h"

#include <cstring>

float_image::float_image(long width, long height) : width(width), height(height), data(new float[width * height]) {
   //empty
}

float_image::float_image(float* data, long width, long height) : width(width), height(height), data(data) {
   //empty
}

float_image::~float_image() {
   //do nothing
}

long float_image::get_width() const {
   return width;
}

long float_image::get_height() const {
   return height;
}

float * float_image::get_data_pointer() {
   return this->data;
}

void float_image::zero_fill() {
   memset(data, 0, width * height * sizeof (float));
}

bool float_image::copy_data_to(float_image * img) {
   if (this->height == img->height && this->width == img->width) {
      memcpy(img->data, this->data, width * height * sizeof (float));
      return true;
   } else {
      return false;
   }
}

void float_image::clear_mem() {
   delete[] data;
   data = 0;
}

float float_image::get_pixel(long iw, long ih) {
   return this->operator()(iw, ih);
}

void float_image::set_pixel(long iw, long ih, float val) {
   (*this)(iw, ih) = val;
}

void delete_float_image(float_image * img) {
   img->clear_mem();
   delete img;
}