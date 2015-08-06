#include "tiles/tile_junction.h"
#include <cmath>

tile_junction::tile_junction() {
   //empty
}

tile_junction::tile_junction(boost::weak_ptr<tile> first, boost::weak_ptr<tile> second, tile::rel_position second_to_first) :first(first), second(second), second_to_first(second_to_first){
//empty
}

tile_junction::~tile_junction() {
   //nothing to do
}

boost::weak_ptr<tile> tile_junction::get_first() {
   return this->first;
}

boost::weak_ptr<tile> tile_junction::get_second() {
   return this->second;
}

tile::rel_position tile_junction::get_relative_position() {
   return this->second_to_first;
}
//!!! ABGLEICH POS RELATIV

float tile_junction::calc_junction_mean_difference() {
   boost::shared_ptr<tile> t1 = this->first.lock();
   boost::shared_ptr<tile> t2 = this->second.lock();
   long H = t1->get_height();
   long W = t1->get_width();

   float diff = 0.0f;

   switch (this->second_to_first) {
      case tile::UP:
      { //[t2]
         for (long i = 0; i < W; i++) { //[t1]
            diff += t1->get_raw_value_at(i, 0) - t2->get_raw_value_at(i, H - 1);
         }
         diff /= float(W);
         break;
      }
      case tile::DOWN:
      { //[t1]
         for (long i = 0; i < W; i++) { //[t2]
            diff += t1->get_raw_value_at(i, H - 1) - t2->get_raw_value_at(i, 0);
         }
         diff /= float(W);
         break;
      }
      case tile::LEFT:
      { //[t2][t1]
         for (long i = 0; i < H; i++) {
            diff += t1->get_raw_value_at(0, i) - t2->get_raw_value_at(W - 1, i);
         }
         diff /= float(H);
         break;
      }
      case tile::RIGHT:
      {//[t1][t2]
         for (long i = 0; i < H; i++) {
            diff += t1->get_raw_value_at(W - 1, i) - t2->get_raw_value_at(0, i);
         }
         diff /= float(H);
         break;
      }
      default:
         DEBUG_PRINTLN("Error in tile_junction::calc_junction_mean_difference: Switch-Case hit default: No legal position committed");
         break;
   }
   diff += t1->get_offset() - t2->get_offset();
   this->mean = diff;
   this->has_mean_calculated = true;
   return diff;
}

float tile_junction::get_mean() {
   return this->mean;
}

bool tile_junction::has_mean() {
   return this->has_mean_calculated;
}

float tile_junction::calc_junction_variance() {
   /* Do not check if variance has been calculated before, this has to be done
    * by user before calling this method ... */
   boost::shared_ptr<tile> t1 = this->first.lock();
   boost::shared_ptr<tile> t2 = this->second.lock();
   boost::shared_ptr<std::vector<float> > differences(new std::vector<float>);

   long H = t1->get_height();
   long W = t1->get_width();

   switch (this->second_to_first) {
      case tile::UP:
      { //[t2]
         for (long i = 0; i < W; i++) { //[t1]
            differences->push_back(t1->get_raw_value_at(i, 0) - t2->get_raw_value_at(i, H - 1));
         }
         break;
      }
      case tile::DOWN:
      { //[t1]
         for (long i = 0; i < W; i++) { //[t2]
            differences->push_back(t1->get_raw_value_at(i, H - 1) - t2->get_raw_value_at(i, 0));
         }
         break;
      }
      case tile::LEFT:
      { //[t2][t1]
         for (long i = 0; i < H; i++) {
            differences->push_back(t1->get_raw_value_at(0, i) - t2->get_raw_value_at(W - 1, i));
         }
         break;
      }
      case tile::RIGHT:
      {//[t1][t2]
         for (long i = 0; i < H; i++) {
            differences->push_back(t1->get_raw_value_at(W - 1, i) - t2->get_raw_value_at(0, i));
         }
         break;
      }
      default:
         DEBUG_PRINTLN("Error in tile_junction::calc_junction_variance: Switch-Case hit default: No legal position committed");
         return -1.0f;
   }
   /* Calculate the median START */
   float mean = 0.0f;
   float N = differences->size();
   for (std::vector<float>::iterator it = differences->begin(); it != differences->end(); ++it) {
      mean += (*it);
   }
   mean /= N; //Divide through #
   /* Calculate the median END */

   /* Calculate the variance START */
   float variance = 0.0f;

   for (std::vector<float>::iterator it = differences->begin(); it != differences->end(); ++it) {
      variance += std::pow((*it) - mean, 2.0); //variance = sum of[(x - µ)^2]
   }
   variance /= N;
   this->variance = variance;
   this->has_variance_calculated = true;
   /* Calculate the variance END */
   return variance;
}

float tile_junction::get_variance() {
   return this->variance;
}

bool tile_junction::has_variance() {
   return this->has_variance_calculated;
}

float tile_junction::calc_junction_squared_difference() {
   boost::shared_ptr<tile> t1 = this->first.lock();
   boost::shared_ptr<tile> t2 = this->second.lock();
   long H = t1->get_height();
   long W = t1->get_width();

   float diff = 0.0f;
   float temp = 0.0f;

   switch (this->second_to_first) {
      case tile::UP:
      { //[t2]
         for (long i = 0; i < W; i++) { //[t1]
            temp = t1->get_value_at(i, 0) - t2->get_value_at(i, H - 1);
            diff += temp*temp;
         }
         diff /= float(W * W);
         break;
      }
      case tile::DOWN:
      { //[t1]
         for (long i = 0; i < W; i++) { //[t2]
            temp = t1->get_value_at(i, H - 1) - t2->get_value_at(i, 0);
            diff += temp*temp;
         }
         diff /= float(W * W);
         break;
      }
      case tile::LEFT:
      { //[t2][t1]
         for (long i = 0; i < H; i++) {
            temp = t1->get_value_at(0, i) - t2->get_value_at(W - 1, i);
            diff += temp*temp;
         }
         diff /= float(H * H);
         break;
      }
      case tile::RIGHT:
      {//[t1][t2]
         for (long i = 0; i < H; i++) {
            temp = t1->get_value_at(W - 1, i) - t2->get_value_at(0, i);
            diff += temp*temp;
         }
         diff /= float(H * H);
         break;
      }
      default:
         DEBUG_PRINTLN("Error in tile_junction::calc_junction_squared_difference: Switch-Case hit default: No legal position committed");
         break;
   }
   return diff;
}

bool tile_junction::is_junction_between(boost::shared_ptr<tile> t1, boost::shared_ptr<tile> t2) {
   if ((this->get_first().lock() == t1 && this->get_second().lock() == t2) || (this->get_first().lock() == t2 && (this->get_second().lock() == t1))) {
      return true;
   } else {
      return false;
   }
}

bool tile_junction::is_junction_between(boost::shared_ptr<tile> t, tile::rel_position pos) {
   if ((this->get_first().lock() == t && this->get_relative_position() == pos) || (this->get_second().lock() == t && this->get_relative_position() == -pos)) {
      return true;
   } else {
      return false;
   }
}

void tile_junction::toString() {
   DEBUG_PRINTLN("First tile is at: w: " << this->first.lock()->get_posW() << "h: " << this->first.lock()->get_posH());
   DEBUG_PRINTLN("Second tile is at: w: " << this->second.lock()->get_posW() << "h: " << this->second.lock()->get_posH());
   DEBUG_PRINTLN("Relative Position: " << this->second_to_first);
}