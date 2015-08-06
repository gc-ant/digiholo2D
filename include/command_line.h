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
 * File:   command_line.h
 * Author: b.steltner
 *
 * Created on 19. Dezember 2014, 11:08
 */

#ifndef COMMAND_LINE_H
#define	COMMAND_LINE_H

/* Include Boost Headers */
//SmartPtr will be included via basic_includes
#include <boost/algorithm/string.hpp>



/* Input & output*/
#include "image/float_image.h"
#include "image/in_out.h"

//FORWARD DECLARATION START
class abstract_tile_unwrapper;
class abstract_tile_merger;

//FORWARD DECLARATION END


class command_line {
public:
    //!Std constructor - does nothing...
    command_line();

    /**
     * Main function call. Gets all necessary parameters and starts the program
     * @param method unwrap & merge or just unwrap
     * @param unwrapper name of the unwrapper to be used
     * @param model name of the model to be used in MLSQU unwrapper
     * @param merger name of the merger to be used
     * @param input input location of the file (for getting the real filename)
     * @param output output location (directory, ending with \\ !). NOTE: May be passed empty!
     * @param dimx width of the image
     * @param dimy height of the image
     * @param tilecount number of tiles in x and y direction
     */
    void execute(std::string method, std::string unwrapper, std::vector<std::string> usettings, std::string merger, std::vector<std::string> msettings, std::string input_path, std::vector<std::string> input, std::string output, int dimx, int dimy, std::vector<int> tilecount);

    virtual ~command_line();
private:

    /**
     * Generate the filename with regards to the parameters used for creating the image
     * @param unwrapper name of the unwrapper used
     * @param merger merger used
     * @param input input location of the file (for getting the real filename)
     * @param output output location (directory, ending with \\ !). NOTE: May be passed empty!
     * @param dimx width of the image
     * @param dimy height of the image
     * @param tilecount number of tiles
     * @return 
     */
    char* create_filename(std::string unwrapper, std::string merger, std::string input, std::string output, int dimx, int dimy, std::vector<int> tilecount);

    /**
     * Method returning the named unwrapper, empty sharedptr if no unwrapper with this name is implemented/supported
     * @param unwrapper name of the unwrapper to be used
     * @param model name of the model function to be used by the MLSQU-unwrapper
     * @return sharedptr to unwrapper
     */
    sharedptr<abstract_tile_unwrapper> get_unwrapper(std::string unwrapper, std::vector<std::string> usettings);

    /**
     * Method returning the named merger, empty sharedptr if no merger with this name is implemented/supported
     * @param merger name of the merger to be used
     * @return sharedptr to merger
     */
    sharedptr<abstract_tile_merger> get_merger(std::string merger, std::vector<std::string> msettings);

    /**
     * Single image unwrap method.
     * @param unwrapper name of the unwrapper to be used
     * @param model to be used by the MLSQU unwrapper
     * @param merger name of the merger to be used
     * @param input path to one image *.raw
     * @param output path to a directory the *.raw should be saved to (MAY be un-initialised => Standard Input File Directory is Output File Directory will be used)
     * @param dimx width of the input image
     * @param dimy height of the input image
     * @param tilecount number of tiles in each direction the image is split into blocks (eg. tilecount 50 => 2500 tiles)
     * @param expert_options
     */
    void unwrap_and_merge(std::string unwrapper, std::vector<std::string> usettings, std::string merger, std::vector<std::string> msettings, std::string input_path, std::vector<std::string> input, std::string output, int dimx, int dimy, std::vector<int> tilecount, std::vector<std::string> expert_options = std::vector<std::string>());

    /**
     * Get the image from the input-file location and read it into the wrapped_img.
     * @param input path to one image *.raw
     * @param wrapped_img sharedptr to empty float image, which will get filled with the input image.
     */
    void cl_read_image(std::string input, sharedptr<row_major_float_image> wrapped_img);

    /**
     * Split the image into tiles and calculate the gradient of the tiles (for debug purposes mainly). Then save the two resulting images.
     */
    void calculate_gradient(int dim, std::string input_path, std::vector<std::string> input, std::string output, int dimx, int dimy, std::vector<int> tilecount);

    //! Same arguments as above...
    bool legit_arguments(std::string method, std::string unwrapper, std::string merger, std::string output, int dimx, int dimy, std::vector<int> tilecount);

    /**
     * Use the pixel-based SRNCP-Merger (mainly for comparing purposes).
     * @param input
     * @param output
     * @param dimx
     * @param dimy
     */
    void pixel_srncp(std::string input_path, std::vector<std::string> input, std::string output, int dimx, int dimy);

    /**
     * This method calls the right measurement methods
     * @param method Defines part of the analyse package used
     * @param input_path 
     * @param input
     * @param dimx
     * @param dimy
     */
    void analyse(std::string method, std::string input_path, std::vector<std::string> input, int dimx, int dimy);
};


#endif	/* COMMAND_LINE_H */

