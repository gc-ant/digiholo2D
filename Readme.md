# Foreword (UPDATE 03/2021)
This package was developed as part of my PhD thesis and is no longer actively maintained (and it hasn't been for a while).

## My Thoughts on The Algorithms
If you are looking at this repo, you probably came from reading the unwrapping paper that I wrote a long time ago. At that time, I had an idea of reviving tile based phase unwrapping by putting a new spin on it. I still think the idea is neat, but there are better general purpose algorithms out there. The tiled based algorithm works well when tuned to specific problems, but there are problems it struggles with (like sharp edges). So if you are just looking for a general purpose phase unwrapping algo, look elsewhere.

If you are interested in the approach from the paper for other reasons, I advise you ultimately write your own implementation. You can play with the code here and take what you like, but see also the remarks in the next section.

## My Thoughts on the Code
The code is bad. Without being too harsh on past me, it is clearly visible that I was an okay Java programmer pretending to be a C++ programmer. Overall structure is very Java like and takes little to no advantage of modern C++ features, plus it is ripe with beginner memory management mistakes (only some of which I corrected in this update).

Furthermore, the code is (in some places) still littered with hardcoded windows file paths. I have cleaned up the code for the standard usecases but there is still loads of hardcoded nastiness in there. Also, there is not a single unit or integration test to be found.

Well, the nice thing is probably that I am a vastly superior software dev and architect now than I was back then (shameless plug for [my devblog here](https://geo-ant.github.io/blog/)).

# Digiholo2D (vers. 0.2)#
A C++11 framework for tile-based phase unwrapping of 2D datasets for digital holography and other applications. This project provides a modular framework for implementation and testing of tile-based phase unwrapping algorithms. It introduces original algorithms presented in [1], as well as a Block-Least-Squares approach described by Strand *et al.* in [2]. Moreover this contains a novel tile merging algorithm based on an idea by Herráez *et. al* [3].

## In a nutshell
"A variety of physical and biomedical imaging techniques, such as digital holography, interferometric synthetic aperture radar (InSAR), or magnetic resonance imaging (MRI) enable measurement of the phase of a physical quantity additionally to its amplitude. However, the phase can commonly only be measured modulo 2pi, as a so called wrapped phase map. Phase unwrapping is the process of obtaining the underlying physical phase map from the wrapped phase. Tile-based phase unwrapping algorithms operate by first tessellating the phase map, then unwrapping individual tiles, and finally merging them to a continuous phase map. They can be implemented computationally efficiently and are robust to noise. However, they are prone to failure in the presence of phase residues or erroneous unwraps of single tiles. To increase the robustness of the tile unwrapping step, we implemented a model-based algorithm that makes efficient use of linear algebra to unwrap individual tiles. Furthermore, we adapted an established pixel-based unwrapping algorithm to create a quality guided tile merger. These original algorithms as well existing ones were implemented in a modular phase unwrapping C++ framework. By examining different combinations of unwrapping and merging algorithms we compared our method to existing approaches. We could show that the appropriate choice of unwrapping and merging algorithms can significantly improve the unwrapped result in the presence of phase residues and unwrap errors. Beyond that, our modular framework allows for efficient design and test of new tile-based phase unwrapping algorithms." [1]

## Tile-based phase unwrapping
For tile-based phase unwrapping, a phase map is tesselated into rectangular tiles of fixed dimensions. Unwrapping is then performed ion a two step process:

1. Unwrapping of individual tiles by a tile unwrapping algorithm (tile_unwrapper).
2. Merging of unwrapped tiles to a continuous surface (tile_merger).

Combining a tile_unwrapper and a tile_merger yields a tile-based phase unwrapping algorithm. Various implementations of tile unwrapping and merging algorithms are provided, see the documentation and corresponding publication [1] for additional information.

# Building from Source (UPDATE 03/2021)
The build system now is [CMake](https://cmake.org/) and the repo uses [conan](https://docs.conan.io/en/latest/getting_started.html) to manage the dependencies. So if you are on a linux machine you can build the code by checking out the repo, `cd`ing into the base folder and from there type

```shell
mkdir build && cd build
conan install ..
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

and you'll get the digiholo2D command line tool.

Probably there is a similar way to do it in Windows, but I don't use Windows for development anymore :)

## Generating the Documentation
The source code in a [doxygen](https://www.stack.nl/~dimitri/doxygen/) compatible format. Doxygen output in HTML and LaTeX is found in the **documentation** directory.


# Using the Command Line Tool

If you use this software please cite reference [1]:

G. Antonopoulos, B. Steltner, A. Heisterkamp, T. Ripken, H. Meyer: "Tile-based two-dimensional phase unwrapping for digital holography using a modular framework", PLoS ONE 10(11): e0143186. (2015),

Extra documentation for the command line tool is in the README.md file located in the dist directory. It was originally written with windows in mind, but the program options are the same for linux.

Assuming you are in the bin directory where the `digiholo2D` executable is located and you have placed the raw image from the sample_images into that directory you can write

```shell
./digiholo2D --path . --output . --width 2560 --height 1920 --tilecount 20 --unwrapper mlsqu --usettings polynom-2-3 --merger srncp wrapped_cluster_32bit_2560x1920.raw
```

The unwrapped image will be output in the same directory.

## FIJI (ImageJ) plugin (win64)
An FIJI (ImageJ) plugin is included in the **imagej** directory. The plugin provides a graphical user interface for the command line tool. It comes packaged with the command line tool as well as a one-click installer. Navigate to the **imagej** directory for more information.

## (Legacy) Win64 Binary (UPDATE 03/2021)
The dist directory contains a Win64 executable compiled in debug mode. This is kept here for legacy reasons and I strongly suggest you build the tool from source.

# References
[1] G. Antonopoulos, B. Steltner, A. Heisterkamp, T. Ripken, H. Meyer: "Tile-based two-dimensional phase unwrapping for digital holography using a modular framework", PLoS ONE 10(11): e0143186. (2015), doi: [10.1371/journal.pone.0143186](http://dx.doi.org/10.1371/journal.pone.0143186)

[2] J. Strand, T. Taxt, A. K. Jain: "Two-dimensional phase unwrapping using a block least-squares method", IEEE transactions on image processing (1999), doi: [10.1109/83.748892](http://dx.doi.org/10.1109/83.748892)

[3] M.A. Herráez, D.R. Burton, M. J. Lalor, M. A. Gdeisat: "Fast two-dimensional phase-unwrapping algorithm based on sorting by reliability following a noncontinuous path", Applied Optics (2002),  doi: [10.1364/AO.41.007437](http://dx.doi.org/10.1364/AO.41.007437)

# License
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
