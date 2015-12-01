#Digiholo2D (vers. 0.2)#
A C++11 framework for tile-based phase unwrapping of 2D datasets for digital holography and other applications. This project provides a modular framework for implementation and testing of tile-based phase unwrapping algorithms. It introduces original algorithms presented in [1], as well as a Block-Least-Squares approach described by Strand *et al.* in [2]. Moreover this contains a novel tile merging algorithm based on an idea by Herráez *et. al* [3].

##In a nutshell##
"A variety of physical and biomedical imaging techniques, such as digital holography, interferometric synthetic aperture radar (InSAR), or magnetic resonance imaging (MRI) enable measurement of the phase of a physical quantity additionally to its amplitude. However, the phase can commonly only be measured modulo 2pi, as a so called wrapped phase map. Phase unwrapping is the process of obtaining the underlying physical phase map from the wrapped phase. Tile-based phase unwrapping algorithms operate by first tessellating the phase map, then unwrapping individual tiles, and finally merging them to a continuous phase map. They can be implemented computationally efficiently and are robust to noise. However, they are prone to failure in the presence of phase residues or erroneous unwraps of single tiles. To increase the robustness of the tile unwrapping step, we implemented a model-based algorithm that makes efficient use of linear algebra to unwrap individual tiles. Furthermore, we adapted an established pixel-based unwrapping algorithm to create a quality guided tile merger. These original algorithms as well existing ones were implemented in a modular phase unwrapping C++ framework. By examining different combinations of unwrapping and merging algorithms we compared our method to existing approaches. We could show that the appropriate choice of unwrapping and merging algorithms can significantly improve the unwrapped result in the presence of phase residues and unwrap errors. Beyond that, our modular framework allows for efficient design and test of new tile-based phase unwrapping algorithms." [1]

##Tile-based phase unwrapping##
For tile-based phase unwrapping, a phase map is tesselated into rectangular tiles of fixed dimensions. Unwrapping is then performed ion a two step process:

1. Unwrapping of individual tiles by a tile unwrapping algorithm (tile_unwrapper).
2. Merging of unwrapped tiles to a continuous surface (tile_merger).

Combining a tile_unwrapper and a tile_merger yields a tile-based phase unwrapping algorithm. Various implementations of tile unwrapping and merging algorithms are provided, see the documentation and corresponding publication [1] for additional information.

##Usage##

If you use this software please cite reference [1]:

G. Antonopoulos, B. Steltner, A. Heisterkamp, T. Ripken, H. Meyer: "Tile-based two-dimensional phase unwrapping for digital holography using a modular framework", PLoS ONE 10(11): e0143186. (2015), 


###FIJI (ImageJ) plugin (win64)###
An FIJI (ImageJ) plugin is included in the **imagej** directory. The plugin provides a graphical user interface for the command line tool. It comes packaged with the command line tool as well as a one-click installer. Navigate to the **imagej** directory for more information.

###Command line tool (win64)###
An executable binary for use under 64-bit Windows systems is located in the **dist** directory. The binary provides a command line interface and more options to interact with the program than the FIJI plugin. Navigate to the **dist** directory for more information.

###Compiling from source###
The source code is provided as a [NetBeans](https://netbeans.org/) project file. Tthe easiest way to compile from source is to open the project in the NetBeans IDE version 8.0.2 or higher. Then edit the project properties so the paths to the included third party libraries and headers match your configuration and compiler toolset. This project depends on free and open source third party libraries, namely the [BOOST C++ Library](https://www.boost.org/) and the [Eigen3](https://eigen.tuxfamily.org/dox-devel/index.html) library for numerical linear algebra. Your compiler must support the C++11 standard.

###Documentation###
The source code in a [doxygen](https://www.stack.nl/~dimitri/doxygen/) compatible format. Doxygen output in HTML and LaTeX is found in the **documentation** directory.

##References##
[1] G. Antonopoulos, B. Steltner, A. Heisterkamp, T. Ripken, H. Meyer: "Tile-based two-dimensional phase unwrapping for digital holography using a modular framework", PLoS ONE 10(11): e0143186. (2015), doi: [10.1371/journal.pone.0143186](http://dx.doi.org/10.1371/journal.pone.0143186) 

[2] J. Strand, T. Taxt, A. K. Jain: "Two-dimensional phase unwrapping using a block least-squares method", IEEE transactions on image processing (1999), doi: [10.1109/83.748892](http://dx.doi.org/10.1109/83.748892)

[3] M.A. Herráez, D.R. Burton, M. J. Lalor, M. A. Gdeisat: "Fast two-dimensional phase-unwrapping algorithm based on sorting by reliability following a noncontinuous path", Applied Optics (2002),  doi: [10.1364/AO.41.007437](http://dx.doi.org/10.1364/AO.41.007437)

##License##
Copyright 2015 G.C. Antonopoulos and B. Steltner, Laser Zentrum Hannover e.V.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this software package except in compliance with the License.
You may obtain a copy of the License at

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.