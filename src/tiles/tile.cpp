#include "tiles/tile.h"
#include "tiles/tile_dimension_exception.h"
#include "tiles/abstract_coordinate_system.h"
#include "model/abstract_function.h"
#include "algorithm/abstract_gradient_calculator.h"
#include "algorithm/wrap.h"
#include "debug/debug_out.h"


tile::tile(): width(0), height(0), tile_offset(0.f) {
   //empty
}

/**
 * Constructor with explicit values at initialisation
 * @param values sharedarray of width*height float-values
 * @param width width of the tile
 * @param height height of the tile
 * @param posH position in the grid
 * @param posW position in the grid
 */
tile::tile(sharedarray<float> values, long width, long height, long posH, long posW) : sa_data(values), width(width), height(height), posH(posH), posW(posW), tile_offset(0.f) {
   //empty
}

tile::~tile() {
   this->sa_data.reset();
}

tile::tile(const tile& other) :width(0), height(0){
   //benutzen des operator=
   *this = other;
}

//! @todo copy all class members that might come in the future, too

tile & tile::operator=(const tile & other) {
   if ((this->width == 0 && this->height == 0) || (this->width == other.width && this->height == other.height)) {
      this->width = other.width; //necessary if this->width == 0
      this->height = other.height;

      this->posH = other.posH;
      this->posW = other.posW;

      this->sa_data.reset();
      this->sa_data = other.sa_data;

      this->ptr_tilegroup = other.ptr_tilegroup;
      this->tile_offset = other.tile_offset;
      return *this;
   } else {
      throw tile_dimension_exception(); // todo add error info and such
   }
}

tile::tile(sharedptr<row_major_float_image> img, long grid_width, long grid_height, long index_w, long index_h) {
   //1. den Bereich in dem Bild aus dem die Pixel in die Tile überführt werden finden
   //daraus ergibt sich auch die tatsächliche Breite und Höhe der Tile.

   //Startindex für Tile in w-direction
   int start_index_w = index_w *grid_width; //inkl
   int end_index_w = (index_w + 1) * grid_width; //exkl

   //Falls die Breite der Tiles kein ganzzahliges Vielfaches vom Bild ist
   //dann werden nur die übriggebliebenen Pixel hier gelinkt.
   if (end_index_w > img->get_width()) {
      end_index_w = img->get_width();
   }

   this->width = end_index_w - start_index_w;

   if (width < 0) {
      width = 0;
   }

   //Startindex für Tile in h-direction
   int start_index_h = index_h *grid_height;
   int end_index_h = (index_h + 1) * grid_height;

   if (end_index_h > img->get_height()) {
      end_index_h = img->get_height();
   }

   this->height = end_index_h - start_index_h;
   if (height < 0) {
      height = 0;
   }

   //PRINTLN("tile: width = "<< this->width << " height="<< this->height);

   //2. Jetzt den Speicher reservieren und die Werte aus dem Bild in die Tile schreiben

   if (width * height != 0) {
      this->sa_data.reset(new float[width * height]);
      for (int iw = 0; iw < width; iw++) {
         for (int ih = 0; ih < height; ih++) {
            this->sa_data[ih * width + iw] = (*img)(start_index_w + iw, start_index_h + ih);
         }
      }
   }
   else
   {
       PRINTLN("PROBLEM: Tile with width 0 was created in file: " << __FILE__ << " at line: " << __LINE__);
   }
   this->posH = index_h;
   this->posW = index_w;
   this->tile_offset = 0.0f;
}

float tile::get_value_at(long iw, long ih)
{
   return this->sa_data[iw + width * ih] + this->tile_offset;
}

float tile::get_raw_value_at(long iw, long ih) {
   return this->sa_data[iw + width * ih];
}

void tile::set_value_at(long iw, long ih, float val) {
   this->sa_data[iw + width * ih] = val - this->tile_offset;
}

void tile::add_value_at(long iw, long ih, float val) {
   this->sa_data[iw + width * ih] += val;
}

float tile::get_offset() {
   return this->tile_offset;
}

long tile::get_height() const {
   return this->height;
}

long tile::get_width() const {
   return this->width;
}

long tile::get_posH() const {
   return this->posH;
}

long tile::get_posW() const {
   return this->posW;
}

tile & tile::add_value(float value) {
   this->tile_offset += value;
   return *this;
}

//void tile::apply_value() {
//   for (long iw = 0; iw < this->width; iw++) {
//      for (long ih = 0; ih < this->height; ih++) {
//         this->set_value_at(iw, ih, this->get_value_at(iw, ih));
//      }
//   }
//   this->tile_offset = 0.0f;
//}

tile & tile::wrap()
{
    for (long i = 0; i < this->height * this->width; i++)
    {
        sa_data[i] = wrapToPi(sa_data[i]);
    }
    return *this; 
}

tile & tile::rewrap(float val)
{
    for (long i = 0; i < this->height * this->width; i++)
    {
        sa_data[i] = wrapToPi(sa_data[i]-val)+val;
    }
    return *this; 
}

bool tile::has_tilegroup() {
   if (this->ptr_tilegroup.expired()) { //true if no reference, false if object is okay
      return false;
   } else {
      return true;
   }
}

bool tile::is_neighbour(boost::shared_ptr<tile> other) {
   long t1_h, t1_w, t2_h, t2_w;
   t1_h = this->get_posH();
   t1_w = this->get_posW();
   t2_h = other->get_posH();
   t2_w = other->get_posW();

   if (t1_h == (t2_h + 1) && t1_w == t2_w) { //rel_pos: UP
      return true;
   } else if (t1_h == (t2_h - 1) && t1_w == t2_w) { //rel_pos: DOWN
      return true;
   } else if (t1_w == (t2_w + 1) && t1_h == t2_h) { //rel_pos: LEFT
      return true;
   } else if (t1_w == (t2_w - 1) && t1_h == t2_h) { //rel_pos: RIGHT
      return true;
   } else {
      return false;
   }
}

boost::shared_ptr<tilegroup> tile::get_tilegroup() {
   return this->ptr_tilegroup.lock();
}

//tile & tile::subtract_function(sharedptr<abstract_function> func, sharedptr<abstract_coordinate_system> coord)
//{
//    for (int ih = 0; ih < this->get_height(); ih++)
//    {
//        for (int iw = 0; iw < this->get_width(); iw++)
//        {
//            this->sa_data[iw + width * ih] -= func->eval(coord->calc_x_at(iw, ih), coord->calc_y_at(iw, ih));
//        }
//    }
//    return *this;
//}
//
//tile & tile::add_function(sharedptr<abstract_function> func, sharedptr<abstract_coordinate_system> coord)
//{
//    for (int ih = 0; ih < this->get_height(); ih++)
//    {
//        for (int iw = 0; iw < this->get_width(); iw++)
//        {
//            this->sa_data[iw + width * ih] += func->eval(coord->calc_x_at(iw, ih), coord->calc_y_at(iw, ih));
//        }
//    }
//    return *this;
//}

tile & tile::set_values(sharedptr<abstract_function> func, sharedptr<abstract_coordinate_system> coordsys) {
   for (int ih = 0; ih < this->get_height(); ih++) {
      for (int iw = 0; iw < this->get_width(); iw++) {
         this->set_value_at(iw, ih, func->eval(coordsys->calc_x_at(iw, ih), coordsys->calc_y_at(iw, ih)));
      }
   }
   return *this;
}

float tile::calc_mean() {
   //Direct access to data array for maximum speed.
   if (this->width * this->height != 0) {
      float mean = 0.f;
      for (int i = 0; i < this->width * this->height; i++) {
         mean += this->sa_data[i];
      }
      mean /= float(this->width * this->height);
      mean += this->get_offset();
      return mean;
   } else {
      //todo throw exception
      return -1.f; 
      PRINTLN("ERROR: Called calc_mean for tile without elements!! Results will be BS!");
   }
}
