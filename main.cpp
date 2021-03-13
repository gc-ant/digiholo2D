/* 
 * File:   main.cpp
 * Author: g.antonopoulos, b.steltner
 *
 * Created on 1. April 2014, 09:19
 */


//#define BOOST_FILESYSTEM_VERSION 3
//#ifndef BOOST_FILESYSTEM_NO_DEPRECATED 
//#define BOOST_FILESYSTEM_NO_DEPRECATED
//#endif
//#ifndef BOOST_SYSTEM_NO_DEPRECATED 
//#define BOOST_SYSTEM_NO_DEPRECATED
//#endif
////#include "boost/filesystem/path_traits.hpp"
//#include "boost/filesystem.hpp"
//#include "boost/"
////#include "boost/system/windows_error.hpp"
//#include "boost/filesystem/operations.hpp"
//#include "boost/filesystem/path.hpp"
//#include "boost/progress.hpp"
//#include <iostream>

#include "basic_includes.h"
#include "command_line.h"
#include <iostream>
#include <algorithm>

/* Include Boost Headers */
//SmartPtr will be included via basic_includes
//#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

/* Include Test and Debug*/
#include "debug/debug_time.h"

void print_img(float_image & img) {

   for (int h = 0; h < img.get_height(); h++) {
      for (int w = 0; w < img.get_width(); w++) {
         printf("%-6.2f", img(w, h)); //6er Spacing mit 2 Nachkommastellen
      }
      putchar('\n');
   }

}

void print_img(boost::shared_ptr<float_image> pimg) {
   print_img(*pimg.get());
}

//changed by b.steltner
/* FORWARD DECLARATIONS START */

/* FORWARD DECLARATIONS END */

//void test_svd_sampling() {

void print_disclaimer()
{
    std::cout << "*****************************************************************************\n";
    std::cout << "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n";
    std::cout << "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n";
    std::cout << "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n";
    std::cout << "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n";
    std::cout << "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n";
    std::cout << "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n";
    std::cout << "THE SOFTWARE.\n";
    std::cout << "*****************************************************************************\n";
}

/**
 * For help regarding arguments of the command line follow "display help" in the 'h'-case, 
 * or run programm with -h or --help via command line.
 * @param argc #arguments in argv
 * @param argv arguments given via command line
 * @return 
 */
int main(int argc, char** argv) {
   
   print_disclaimer();
    
   try {
      int dimx, dimy;
      std::vector<int> tilecount;
      std::string method;
      std::string input_path;
      std::vector<std::string> input_file;
      std::string output;
      std::string unwrapper;
      std::vector<std::string> usettings;
      std::string merger;
      std::vector<std::string> msettings;

      /* For multiple imports via 1 command line call see http://www.boost.org/doc/libs/1_57_0/doc/html/program_options/tutorial.html "option details" positional options... 
       and change input to input-path<std::string> & input-file vector<std::string> and embrace cla->execute (at the end of this main) with a 
       * for(input-file iterator if_it) cla->execute(bla, bla, input-path + (*if_it), bla, bla); */

      boost::program_options::options_description descriptions("Allowed options: \nMultiple options eg. msettings via multiple calls: \n\"-n [setting 1] -n [setting 2]\" (without \" and \"[]\")");
      descriptions.add_options()
            ("help,h", "See this message")
            ("method,p", boost::program_options::value<std::string>(&method)->default_value("merge"), "\"merge\", \"merge-t\" (time-measurement), \"psnr\" (quality-measure)")
            ("path,i", boost::program_options::value<std::string>(&input_path), "Input directory (eg. C:\\Directory)")
            ("file,f", boost::program_options::value<std::vector<std::string> > (&input_file), "Input file(s) (eg. a.raw b.raw)[possible w/o -f ]")
            ("output,o", boost::program_options::value<std::string>(&output), "Output path (eg. C:\\Output\\)")
            ("width,x", boost::program_options::value<int>(&dimx)->default_value(2560), "Width (pixel) of the image")
            ("height,y", boost::program_options::value<int>(&dimy)->default_value(1920), "Height (pixel) of the image")
            ("tilecount,t", boost::program_options::value<std::vector<int> >(&tilecount), "Number of blocks the image is tesselated (-t 40)")
            ("unwrapper,u", boost::program_options::value<std::string>(&unwrapper), "Unwrapper name: \"strand\" o. \"mlsqu\"")
            ("usettings,v", boost::program_options::value<std::vector<std::string> > (&usettings), "Setting(s) for unwrapper (multi-option | \"help\")")
            ("merger,m", boost::program_options::value<std::string>(&merger), "Merger name: \"simple\", \"strand\", \"srncp\", \"simann\"")
            ("msettings,n", boost::program_options::value<std::vector<std::string> > (&msettings), "Setting(s) for the merger (multi-option | \"help\")");

      boost::program_options::positional_options_description pos_optional;
      pos_optional.add("file", -1);

      boost::program_options::variables_map vm;
      boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(descriptions).positional(pos_optional).run(), vm);
      boost::program_options::notify(vm);
      PRINTLN("");
      if (vm.count("help") == 1) {
         PRINTLN(descriptions);
         return 1;
      }
      if (vm.count("method") == 1) {
         PRINTLN("-p: " << method);
      } else {
         PRINTLN("-p  not given. Nothing to do. Aborting program...");
         return 0;
      }
      if (vm.count("path") == 0) {
         PRINTLN("-i  not given. No input-path. Using ./ to process. (May not be implemented yet..");
         //         return 0;
      }
      if (vm.count("file") == 0) {
         PRINTLN("-f  not given. No file to process. Aborting programm");
         //         return 0; 
      } else {
         PRINTLN("-f  given with " << input_file.size() << " argument(s).");
      }
      if (vm.count("tilecount") == 1) {
         if (tilecount.size() == 2) {
            PRINTLN("-t: " << tilecount.at(0) << " " << tilecount.at(1));
         } else if(tilecount.size() == 1) {
            PRINTLN("-t: " << tilecount.at(0));
         } else   {
            PRINTLN("-t  option called with too many arguments: " << tilecount.size() << ". 1 or 2 supported");
         }
      }
      if (vm.count("width") == 1) {
         PRINTLN("-x: " << dimx);
      }
      if (vm.count("height") == 1) {
         PRINTLN("-y: " << dimy);
      }
      //output, width, height not neccessary
      if (vm.count("unwrapper") == 1) {
         PRINTLN("-u: " << unwrapper);
         if (vm.count("usettings") == 1) {
            PRINTLN("-v  given with " << usettings.size() << " argument(s)");
         }
      }
      if (vm.count("merger") == 1) {
         PRINTLN("-m: " << merger);
         if (vm.count("msettings") == 1) {
            PRINTLN("-n  given with  " << msettings.size() << " argument(s)");
         }
      }
      PRINTLN("");
      /* This is where the whole programm will start */
      command_line * cla = new command_line();
      cla->execute(method, unwrapper, usettings, merger, msettings, input_path, input_file, output, dimx, dimy, tilecount);
      delete cla;
      /* End */
   } catch (std::exception& e) {
      PRINTLN("Error: " << e.what());
   } catch (...) {
      PRINTLN("Exception of unknown type");
   }
   return 0;
}
