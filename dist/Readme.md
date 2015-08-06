#Digiholo2D command line tool#
An executable binary file named **digiholo2D_v2.exe** for 64-bit Windows systems was compiled using the MinGW64 collection and is provided herein. The tool lets the user select a tile unwrapping algorithm and a tile merger. Tile unwrapping and merging algorithms are described in detail in [1]. A brief recap is given in the next section.

##Algorithms##

###Tile unwrappers###
These Algorithms unwrap individual tiles independent of each other.

**Strand's tile unwrapper** (strand_tile_unwrapper.h): Described by Strand *et al.* in [2]. Unwraps tiles choosing a rewrap constant that minimizes the number of phase jumps. The algorithm takes the number of minimization steps as a parameter.

**MLSQU tile unwrapper** (least_squares_grad_unwrapper.h): New algorithm developed in [1]. It extends Strand's tile merger by fitting a model function to the phase distribution. This allows for larger tiles. In the current implementation this algorithm uses 2D mixed order polynomials and takes the degree in x- and y-direction as a parameter. Furthermore it performs one unwrapping step using Strand's tile unwrapper. Thus it also needs a number of minimization steps.

###Tile mergers###
**Strand's tile merger**: Described by Strand *et al.* in [2]. Unidirectional tile merger that requires one pass over the image.

**SRNCP tile merger**: Tile merger developed in [1] based on an idea by Herráez *et al.* [3]. More robust against residues than Strand's merger.

The package further provides tile mergers based on simulated annealing and a combination of simulated annealing and flood filling. These should be considered experimental. Please refer to the source code for more information.

##Usage##
The command line tool **digiholo2D_v2.exe** requires images in raw binary format without headers. It requires a 32-bit floating point data type in little-endian order (Intel byte order). This will also be the output format.

For a GUI interface please navigate to the **imagej** directory and take a look at the ImageJ /FIJI plugin.

###Command line options###

* *-h / --help*: Display help.
* *-i / --path*: Directory that contains the input wrapped phase maps.
* *-o / --output*: Existing directory in which the unwrapped results will be saved.
* *-x / --width*: Width of the input image(s) in pixels.
* *-y / --height*: Image height in pixels.
* *-t / --tilecount*: Number of tiles into which the phase map is divided. Use '-t 40' to divide into 40x40 tiles and '-t 30 -t 20' for 30x20 tiles.
* *-u / --unwrapper*: Name of tile unwrapping algorithm. Possible options are "strand", "mlsqu". For the simulated annealing algorithms refer to the source code.
* *-v / --usettings*: Further arguments for the tile unwrapping algorithm (see below).
* *-m / --merger*: Name of tile merging algorithm. Possible options are "strand" and "srncp".
* *-n / --msettings*: Further arguments for the tile merging algorithm. Default settings are fine, refer to source code for more information.

Input files can be passed to the program by specifying the filename at the end of the command line. Unwrapping of multiple input files is supported. All input files must have same size, see examples below.

This concludes the list of the most important command line options for day to day use. For some further options refer to the source code.

####Options for tile unwrapping algorithms (--usettings)####
The --usettings or -v option passes more arguments to the tile unwrapping algorithm. It can be followed by the following keywords:

* *--usettings steps-[number]*: Sets the number of steps for the minimization of discontinuities to [number]. Both the MLSQU and the Strand tile unwrapper accept this option. This is due to the fact, that the MLSQU tile unwrapper includes the Strand tile unwrapper.
* *--usettings polynom-[deg_x]-[deg_y]*: Only for MLSQU unwrapper. Sets the model function f(x) to a mixed order polynomial with maximum degree in x and y of [deg_x] and [deg_y] respectively. E.g. using 'polynom-1-2' will result in f(x) = c1 + c2*x + c3*y + c5*xy + c4*x*y^2.

The --usettings option has to precede each option passed to the unwrapper, see examples below.




###Examples###
	digiholo2D_v2.exe --path C:\inputfolder\ --output C:\outputfolder\ --width 100 --height 200 --tilecount 20 --unwrapper strand --usettings steps-20 --merger srncp wrap.raw

Unwraps an image named 'wrap.raw' of dimensions 100x200 pixels, located in 'C:\inputfolder'. The output will be saved to 'C:\outputfolder'. This folder has to exists. Unwrapping will be performed using a tesselation into 20x20 tiles. Individual tiles will be unwrapped using Strand's tile unwrapper. The merging process will be performed using the tile-based SRNCP algorithm.

	digiholo2D_v2.exe --path C:\inputfolder\ --output C:\outputfolder\ --width 100 --height 200 --tilecount 20 --unwrapper strand --usettings steps-20 --merger srncp A.raw B.raw C.raw

Unwraps three images 'A.raw', 'B.raw' and 'C.raw' with the same settings as above. All images must have the same dimensions of 100x200 pixels.

	digiholo2D_v2.exe --path C:\inputfolder\ --output C:\outputfolder\ --width 100 --height 200 --tilecount 20 --unwrapper mlsqu --usettings polynom-2-3 --usettings strand-20 --merger srncp A.raw B.raw C.raw

Same as above but using the MLSQU tile unwrapper with a polynomial of maximum degree in x and y of 2 and 3, respectively. 20 minimization steps are used for the integrated Strand tile unwrapper.

###Command line arguments###


##References##
[1] G. Antonopoulos, B. Steltner, A. Heisterkamp, T. Ripken, H. Meyer: "Tile-based two-dimensional phase unwrapping for digital holography using a modular framework", *submitted* (2015)

[2] J. Strand, T. Taxt, A. K. Jain: "Two-dimensional phase unwrapping using a block least-squares method", IEEE transactions on image processing (1999), doi: [10.1109/83.748892](http://dx.doi.org/10.1109/83.748892)

[3] M.A. Herráez, D.R. Burton, M. J. Lalor, M. A. Gdeisat: "Fast two-dimensional phase-unwrapping algorithm based on sorting by reliability following a noncontinuous path", Applied Optics (2002),  doi: [10.1364/AO.41.007437](http://dx.doi.org/10.1364/AO.41.007437)