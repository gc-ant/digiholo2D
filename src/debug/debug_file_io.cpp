/* 
 * File:   debug_file_io.h
 * Author: b.steltner
 *
 * Created on 2. Dezember 2014, 14:14
 */

#include "debug/debug_file_io.h"
#include "debug/debug_out.h"
#include <boost/lexical_cast.hpp>

debug_file_io::debug_file_io() {
   //empty
}

debug_file_io& debug_file_io::operator=(const debug_file_io&) {
   //empty
   return *this;
}

debug_file_io::debug_file_io(std::string filename) : myfile(filename, std::ofstream::app)   {
 //empty
}

debug_file_io::~debug_file_io(){
        this->myfile.close(); 
}

void debug_file_io::append_to_file(std::string text)  {
    if(myfile.is_open()) {
      this->myfile << text; 
   } else   {
       PRINTLN("Cannot save to file..");
   }
}

void debug_file_io::clear_file()   {
   
}

void debug_file_io::read_file()     {
   
}