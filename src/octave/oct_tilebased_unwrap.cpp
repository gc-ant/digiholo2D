#include "oct.h"


#include "col_major_image.h"
#include "block_srncp/grad_fit_tile_unwrapper.h"
//#include "block_srncp/srncp_tile_merger.h"

//#include "block_srncp/tile_merge_unwrapper.h"

/**
 * Funktion, um den Phase unwrap in Octave nutzbar zu machen.
 */
DEFUN_DLD(oct_tilebased_unwrap, args, nargout, "2D Phase Unwrap by G. Antonopoulos.")
{
    
    //Arguments: srncp_unwrap(wrapped_phase);
    //Dabei muss wrapped_phase eine matrix sein.
    
    int n_argin = args.length();
    
    //Argumente Checken
    if(n_argin > 2)
    {
        error("Function expects one or two arguments: First a real matrix and [second an integer number].\n");
    }
    if(args(0).is_real_matrix() == false)
    {
        error("First argument must be a real matrix.\n");
    }
    if(args(1).is_integer_type() == false)
    {
        error("Second argument must be an integer type.\n");
    }
    //Wenn zweites Argument angegeben ist, dann hier die N_width für die tiles herauslesen
    int N_width;
    if(n_argin == 2)
    {
        N_width = args(0).int_value();
    }
    else
    {
        N_width = 80;
    }
    
    FloatMatrix wrapped_matrix     = args(0).float_matrix_value();
    int img_width = wrapped_matrix.dim1();
    int img_height = wrapped_matrix.dim2();
    
    float fW = float(img_width);
    float fH = float(img_height);
    
    //Anzahl der Kästchen in h Richtung anpassen an Anzahl in W-richtung.
    int N_height = int(fH/fW*((float)N_width));
    
    //Behälter für das Unwrapped Bild zur Verfügung stellen
    FloatMatrix unwrapped_matrix(img_width, img_height);
    
    col_major_float_image wrapped_img(wrapped_matrix.fortran_vec(), img_width, img_height);
    col_major_float_image unwrapped_img(unwrapped_matrix.fortran_vec(), img_width, img_height);    
    
    //Create the unwrapper
    grad_fit_tile_unwrapper gfu;
    //srncp_tile_merger sm;

    return octave_value();
}


