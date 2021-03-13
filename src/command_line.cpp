/* 
 * File:   command_line.h
 * Author: b.steltner
 *
 * Created on 19. Dezember 2014, 11:08
 */

/* Include Merger & Unwrapper */
#include "algorithm/abstract_tile_unwrapper.h"
#include "algorithm/abstract_tile_merger.h"

#include "algorithm/grad_fit_tile_unwrapper.h"
#include "algorithm/strand_tile_unwrapper.h"
#include "algorithm/least_squares_grad_unwrapper.h"

#include "algorithm/simple1d_tile_merger.h"
#include "algorithm/strand_tile_merger.h"
#include "algorithm/srncp_tile_merger.h"
#include "algorithm/simulated_annealing_merger.h"
#include "algorithm/simulated_annealing_floodfill_merger.h"

//#include "srncp/srncp_unwrap.h"

/* Include analysing */
#include "analyse/measure_psnr.h"
#include "analyse/measure_pixel_energy.h"

/* Include block_based approach */
/* Gets included via the numerous unwrapper/merger */

/* Include Model & Functions */
#include "model/monomial_function.h"
#include "model/model_function.h"

#include "command_line.h"
#include "debug/debug_out.h"
#include "debug/debug_file_io.h"
#include "debug/debug_time.h"

#include "boost/filesystem.hpp"

command_line::command_line() {
}

command_line::~command_line() {
    //empty
}

void command_line::execute(std::string method, std::string unwrapper, std::vector<std::string> usettings, std::string merger, std::vector<std::string> msettings, std::string input_path, std::vector<std::string> input, std::string output, int dimx, int dimy, std::vector<int> tilecount) {
    //TODO: "Repair" input path - closing // 
    //TODO: "Repair" input_file - closing .raw

    //TODO: Check for legit arguments doesnt make sense with that many options ... 
    //if (!legit_arguments(method, unwrapper, merger, output, dimx, dimy, tilecount)) {
    //Legit_arg-method creates error-messages, no need to do it here...
    //        exit(0);
    //  }
    if (method.compare("merge") == 0) { // unwrap und merge 
        this->unwrap_and_merge(unwrapper, usettings, merger, msettings, input_path, input, output, dimx, dimy, tilecount);
    } else if (method.substr(0, 6).compare("merge-") == 0) {
        std::vector<std::string> expert;
        std::string copy = method;
        while ((copy.find_first_of('-') != std::string::npos)) {
            expert.emplace_back(copy.substr(0, copy.find_first_of('-')));
            copy = copy.substr(copy.find_first_of('-') + 1);
        }
        if (!copy.empty()) expert.push_back(copy);
        this->unwrap_and_merge(unwrapper, usettings, merger, msettings, input_path, input, output, dimx, dimy, tilecount, expert);
    } else if ((method.find_first_of('-') != std::string::npos) && method.substr(0, method.find_first_of('-')).compare("analyse") == 0) {
        //catch possible error if no '-' is in the string. If multi-option: Check if first is "analyse".
        this->analyse(method.substr(method.find_first_of('-') + 1), input_path, input, dimx, dimy);
   // } else if (method.compare("srncp") == 0) { //excluded, see pixel_srncp method
   //     this->pixel_srncp(input_path, input, output, dimx, dimy);
    } else if (method.compare("grad_x") == 0) {// calculate x gradient
        this->calculate_gradient(0, input_path, input, output, dimx, dimy, tilecount);
    } else if (method.compare("grad_y") == 0) { // calculate y gradient
        this->calculate_gradient(1, input_path, input, output, dimx, dimy, tilecount);
    } else {
        PRINTLN("Error: method is empty. I dont know what to do with: " << method);
    }
}

bool command_line::legit_arguments(std::string method, std::string unwrapper, std::string merger, std::string output, int dimx, int dimy, std::vector<int> tilecount) {
    bool legit_arguments = true;
    if (dimx <= 0) {
        legit_arguments = false;
        PRINTLN("Error: dimx has unusual value: " << dimx);
    }
    if (dimy <= 0) {
        legit_arguments = false;
        PRINTLN("Error: dimy has unusual value: " << dimy);
    }
    if (tilecount.size() != 1 && tilecount.size() != 2) { //zero is allowed
        legit_arguments = false;
        PRINTLN("Error: tilecount has unusual size: " << tilecount.size());
    }
    return legit_arguments;
}

/**
 * Generate the filename with regards to the parameters used for creating the image
 * @param unwrapper name of the unwrapper used
 * @param merger name of the merger used
 * @param input input location of the file (for getting the real filename)
 * @param output output location (directory, ending with a directory separator !). NOTE: May be passed empty!
 * @param dimx width of the image
 * @param dimy height of the image
 * @param tilecount number of tiles
 * @return 
 */
std::string command_line::create_filename(std::string unwrapper, std::string merger, std::string input, std::string output, int dimx, int dimy, std::vector<int> tilecount) {
    std::string name;
    std::string directory_separator (1,boost::filesystem::path::preferred_separator);
    unsigned last_slash = input.find_last_of(directory_separator); //TODO: Generalize for every OS -using boost::filesystem maybe?
    if (output.empty()) {
        name += (input.substr(0, last_slash));
        name += directory_separator+"Unwrapped"+directory_separator;
    } else {
        if (output.substr(output.length() - 1, output.length() - 0).compare(directory_separator.c_str()) == 0) {
            name += output;
        } else name += output + directory_separator;
    }
    name += unwrapper;
    name += "_";
    name += merger;
    name += "_";
    name += boost::lexical_cast<std::string>(dimx);
    name += "x";
    name += boost::lexical_cast<std::string>(dimy);
    name += "_";
    for (std::vector<int>::iterator it = tilecount.begin(); it != tilecount.end(); ++it) {
        name += boost::lexical_cast<std::string>(*it) + "_";
    }
    //   unsigned last_slash = input.find_last_of("\\");
    name += input.substr(last_slash + 1, input.length() - last_slash - 5); //Extract the filename of the given file minus path minus ending
    name += ".raw";
    PRINTLN("Save file to: " << name);

    return name;
}

sharedptr<abstract_tile_unwrapper> command_line::get_unwrapper(std::string unwrapper, std::vector<std::string> usettings) {
    if (unwrapper.empty()) {
        //do nothing
    } else if (unwrapper.compare("strand") == 0) {
        sharedptr<strand_tile_unwrapper> my_unwrapper(new strand_tile_unwrapper(usettings));
        return my_unwrapper;
        //   } else if (strcmp(unwrapper, "grad") == 0) {
        //      sharedptr<smart_grad_fit_tile_unwrapper> my_unwrapper(new smart_grad_fit_tile_unwrapper());
        //      return my_unwrapper;
    } else if (unwrapper.compare("mlsqu") == 0) {
        sharedptr<least_squares_grad_unwrapper> my_unwrapper(new least_squares_grad_unwrapper(usettings));
        return my_unwrapper;
    } else {
        PRINTLN("Error: Only strand & mlsqu - Unwrapper supported: " << unwrapper);
        exit(0);
    }
    sharedptr<abstract_tile_unwrapper> my_unwrapper;

    return my_unwrapper;
}

sharedptr<abstract_tile_merger> command_line::get_merger(std::string merger, std::vector<std::string> msettings) {
    if (merger.empty()) {
        //do nothing
    } else if (merger.compare("srncp") == 0) {
        sharedptr<srncp_tile_merger> my_merger(new srncp_tile_merger(msettings));
        return my_merger;
    } else if (merger.compare("strand") == 0) {
        sharedptr<strand_tile_merger>my_merger(new strand_tile_merger());
        return my_merger;
    } else if (merger.compare("simple") == 0) {
        sharedptr<simple1d_tile_merger> my_merger(new simple1d_tile_merger(true, true, true));
        return my_merger;
    } else if (merger.compare("simann") == 0) {
        sharedptr<simulated_annealing_merger> my_merger(new simulated_annealing_merger(msettings));
        return my_merger;
    } else if (merger.compare("simannflood") == 0) {
        sharedptr<simulated_annealing_floodfill_merger> my_merger(new simulated_annealing_floodfill_merger(msettings));
        return my_merger;
    } else {
        PRINTLN("Error: Only srncp, strand, simple & simann - Merger supported: " << merger);
        exit(0);
    }
    sharedptr<abstract_tile_merger> my_merger;

    return my_merger;
}

void command_line::unwrap_and_merge(std::string unwrapper, std::vector<std::string> usettings, std::string merger, std::vector<std::string> msettings, std::string input_path, std::vector<std::string> input_file, std::string output, int dimx, int dimy, std::vector<int> tilecount, std::vector<std::string> expert_options) {
    bool timing = false;
    float time_u = 0.f;
    float time_m = 0.f;
    for (std::string option : expert_options) { //ForEach loop
        if (option.compare("t") == 0) {
            timing = true;
        }
    }

    if (timing) {
        debug_file_io file_io("timings.txt");
        std::string temp = "\n" + merger + " " + unwrapper + " ";
        temp += boost::lexical_cast<std::string>(tilecount.at(0)) + " " + boost::lexical_cast<std::string>(dimx) + " " + boost::lexical_cast<std::string>(dimy);
        file_io.append_to_file(temp);
    }

    /* Preinitalize variables */
    std::string input;
    std::vector<std::string>::iterator file_it;

    sharedptr<abstract_tile_unwrapper> my_unwrapper;
    if (!unwrapper.empty() && unwrapper.compare("none") != 0) {
        my_unwrapper = this->get_unwrapper(unwrapper, usettings);
    }
    sharedptr<abstract_tile_merger> my_merger;
    if (!merger.empty() && merger.compare("none") != 0) {
        my_merger = this->get_merger(merger, msettings);
    }

    sharedptr<row_major_float_image> wrapped_img = create_row_major_float_image(dimx, dimy);

    for (file_it = input_file.begin(); file_it != input_file.end(); ++file_it) {
        //Assemble input file path (and try to correct errors with ending backslashes)
        if (input_path.back() != boost::filesystem::path::preferred_separator) input_path += boost::filesystem::path::preferred_separator;
        input = input_path + (*file_it);

        /* Grab the image */
        wrapped_img->zero_fill();
        this->cl_read_image(input, wrapped_img); //Read the image dependent of the image type

        sharedptr<tiled_image> sti(new tiled_image(wrapped_img, tilecount.at(0), (tilecount.size() == 2 ? tilecount.at(1) : tilecount.at(0)))); //ternary operator: If tilecount.size == 2, then use tilecount[1], otherwise tilecount[0]

        if (!unwrapper.empty() && unwrapper.compare("none") != 0) {
            debug_time *time = new debug_time();
            sti->unwrap_tiles(my_unwrapper);
            time_u = time->get_time();
            unwrapper = my_unwrapper->get_name();
            if (timing) {
                unwrapper = unwrapper.substr(0, unwrapper.find("_ti"));
                unwrapper += "_tiu_" + boost::lexical_cast<std::string>(time_u).substr(0, 5);
            }
            PRINTLN("Runtime Unwrapper: ");
            delete time;
        }
        if (!merger.empty() && merger.compare("none") != 0) {
            debug_time *time = new debug_time();
            my_merger->merge_tiles(sti);
            time_m = time->get_time();
            merger = my_merger->get_name();
            if (timing) {
                merger = merger.substr(0, merger.find("_ti"));
                merger += "_tim_" + boost::lexical_cast<std::string>(time_m).substr(0, 5);
            }
            PRINTLN("Runtime Merger: ");
            delete time;
        }
        PRINTLN("");
        sharedptr<row_major_float_image> unwrapped_img = sti->convert_to_float_image();
        if (!write_image(create_filename(unwrapper, merger, input, output, dimx, dimy, tilecount), unwrapped_img.get())) {
            PRINTLN("Error: Could not save file. \n Note: This programm has no cross-platform creation of folders (yet) supported.");
            PRINTLN("If you see this error message, please check if the specified output (or .\\Unwrapped\\ when no -o option called) is an EXISTING FOLDER.");
            return;
        } else PRINTLN("----- Finished merging -----");
        //save data to file here
        if (timing) {
            debug_file_io file_io("timings.txt"); //should append to file
            std::string app = " " + boost::lexical_cast<std::string>(time_u).substr(0, 5) + " " + boost::lexical_cast<std::string>(time_m).substr(0, 5);
            file_io.append_to_file(app);
        }
    }
}

void command_line::cl_read_image(std::string input, sharedptr<row_major_float_image> wrapped_img) {
    if (!read_image<float>(&input[0], wrapped_img)) {
        PRINTLN("Error: Could not read 32-bit float file.");
        exit(0);
    }
}

void command_line::calculate_gradient(int dim, std::string input_path, std::vector<std::string> input_file, std::string output, int dimx, int dimy, std::vector<int> tilecount) {
    std::string input;
    std::vector<std::string>::iterator file_it;
    for (file_it = input_file.begin(); file_it != input_file.end(); ++file_it) {
        //Assemble input file path (and try to correct errors with ending backslashes)
        if (input_path.back() != boost::filesystem::path::preferred_separator) {
            input_path += boost::filesystem::path::preferred_separator;
        }
        input = input_path + (*file_it);

        /* Grab the image */
        sharedptr<row_major_float_image> wrapped_img = create_row_major_float_image(dimx, dimy);
        wrapped_img->zero_fill();
        if (!read_image<float>(&input[0], wrapped_img)) {
            PRINTLN("Error: Could not read file.");
            return;
        }
        /* */
        int tilecount_y;
        if (tilecount.size() == 2) tilecount_y = tilecount.at(1);
        else tilecount_y = tilecount.at(0);

        sharedptr<tiled_image> sti(new tiled_image(wrapped_img, tilecount.at(0), tilecount_y));

        sti->gradient(dim); //calc gradient in special dim
        std::string dim_str;
        if (dim != 1) {
            dim_str = "x";
        } else {
            dim_str = "y";
        }
        sharedptr<row_major_float_image> grad_img = sti->convert_to_float_image();
        if (!write_image(create_filename("grad", dim_str, input, output, dimx, dimy, tilecount), grad_img.get())) {
            PRINTLN("Error: Could not save file.");
            return;
        } else {

            PRINTLN("Finished merging");
        }
    }
}

/*
 * EXCLUDED IN THIS RELEASE
 * this used to be code for the pixelbased SRNCP unwrapper and is not included in this
 * release due to licensing reasons. Go to https://www.ljmu.ac.uk/about-us/faculties/faculty-of-technology-and-environment/general-engineering-research-institute/projects/phase-unwrapping
 * to obtain the code from its authors.
 */
//void command_line::pixel_srncp(std::string input_path, std::vector<std::string> input_file, std::string output, int dimx, int dimy) {
//    std::string input;
//    std::vector<std::string>::iterator file_it;
//    for (file_it = input_file.begin(); file_it != input_file.end(); ++file_it) {
//        //Assemble input file path (and try to correct errors with ending backslashes)
//        if (input_path.back() != '\\') {
//            input_path += "\\";
//        }
//        input = input_path + (*file_it);
//
//        sharedptr<row_major_float_image> wrapped_img = create_row_major_float_image(dimx, dimy);
//        wrapped_img->zero_fill();
//        this->cl_read_image(input, wrapped_img); //Read the image dependent of the image type
//
//        sharedptr<row_major_float_image> unwrapped_img = create_row_major_float_image(dimx, dimy);
//        //   sharedptr<f> unwrapped_img = create_row_major_float_image(dimx, dimy);
//
//        //     float_image * unwrapped_phi = new row_major_float_image(dimx, dimy);
//        //   sharedptr<float_image> unwrappi = boost::static_pointer_cast<float_image>(unwrapped_img);
//
//        srncp_unwrapper my_unwrapper;
//        debug_time *time = new debug_time();
//        my_unwrapper.unwrap(wrapped_img.get(), unwrapped_img.get());
//        PRINTLN("Pixel-based SRNCP took:");
//        time->get_time();
//        delete time;
//        //   my_unwrapper->unwrap(wrappi.get(), unwrappi.get());
//        std::string empty = "";
//
//        if (!write_image(create_filename(empty + "srncp", empty, input, output, dimx, dimy, std::vector<int>()), unwrapped_img.get())) {
//            PRINTLN("Error: Could not save file.");
//            return;
//        } else PRINTLN("Finished merging");
//    }
//}

void command_line::analyse(std::string method, std::string input_path, std::vector<std::string> input_file, int dimx, int dimy) {
    std::string input, input_noisy;

    if (method.compare("pixel") == 0) {
        for (std::string file_name : input_file) { //measure every file
            if (input_path.back() != boost::filesystem::path::preferred_separator) input_path += boost::filesystem::path::preferred_separator;
            input = input_path + file_name;

            sharedptr<row_major_float_image> wrapped_img = create_row_major_float_image(dimx, dimy);
            wrapped_img->zero_fill();
            this->cl_read_image(input, wrapped_img);

            sharedptr<measure_pixel_energy> measure(new measure_pixel_energy());

            PRINTLN("*--------------------------------------------------------------------------*");
            PRINTLN("PSNR of image: ");
            PRINTLN(file_name);
            PRINTLN("Pixel_Energy: " << measure->calc(wrapped_img));
            PRINTLN("*--------------------------------------------------------------------------*");
        }
    } else if (method.compare("psnr") == 0) {
        if (input_file.size() == 2) {
            if (input_path.back() != boost::filesystem::path::preferred_separator) input_path += boost::filesystem::path::preferred_separator;
            input = input_path + input_file.front();
            input_noisy = input_path + input_file.back();

            sharedptr<row_major_float_image> original = create_row_major_float_image(dimx, dimy);
            sharedptr<row_major_float_image> noisy = create_row_major_float_image(dimx, dimy);
            this->cl_read_image(input, original);
            this->cl_read_image(input_noisy, noisy);

            sharedptr<measure_psnr> measure(new measure_psnr(noisy, original, 32));
            PRINTLN("*--------------------------------------------------------------------------*");
            PRINTLN("PSNR of images: ");
            PRINTLN("#1: " << input_file.front());
            PRINTLN("#2: " << input_file.back());
            PRINTLN("PSNR: " << measure->calculate_psnr() << " dB");
            PRINTLN("*--------------------------------------------------------------------------*");

        } else {
            PRINTLN("Number of input file(s) is unequal 2. Please provide \"original\" image first, \"noisy\" image second.");
        }
    }
}
