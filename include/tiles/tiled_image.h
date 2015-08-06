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
 * File:   tiled_image.h
 * Author: g.antonopoulos
 *
 * Created on 16. Oktober 2014, 15:57
 */

#ifndef TILED_IMAGE_H
#define	TILED_IMAGE_H


#include <boost/optional.hpp>
#include "tiles/tile.h"
#include "algorithm/second_order_gradient.h"


//FORWARD DECLARATION START
class tile_junction;
class tilegroup;
class abstract_tile_unwrapper;
class abstract_gradient_calculator;
//FORWARD DECLARATION END

/**
 * @class tiled_image Diese Klasse ersetzt tesselated image.
 * 
 * Sie verwaltet ein 2D Array von tiles, die zusammengesetzt ein Bild ergeben. 
 * Dabei erfolgt die Verwaltung des 2D Arrays intern über eine Indizierung eines
 * 1D Vektors aus strong pointers auf die tiles. Damit kann die dynamische
 * Speicherverwaltung von boost mit dynamischen Arrays kombiniert werden.
 * 
 * Ein tiled_image wird mit den Daten aus einem float_image initialisiert,
 * außerdem werdend die Anzahl der tiles in w und h Richtung übergeben in die 
 * das Bild zerlegt werden soll. Weil jede tile sich eine lokale Kopie
 * der betreffenden Daten erstellt ist das tiled_image in diesem Sinne
 * auch eine Kopie des gegebenen float_image. Die Operationen auf dem
 * tiled_image verändern das Originalbild also nicht.
 * 
 * 
 * 
 */
class tiled_image {
    friend void add_tile_to_group(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);
private:

    /**
     * Verwaltung der tiles in einem dynamischen 1D Array. Die Tile am Index
     * (w,h) soll an der Position h*WIDTH+w sitzen, wobei WIDTH die Anzahl der
     * Tiles in w-Richtung ist.
     */
    std::vector< boost::shared_ptr<tile> > vc_tile_array;

    /**
     * This vector holds the junctions createad in the method "create_all_junctions" provided with this class. 
     * The method inserts the DOWN junction of tile i at index i and afterwards the right junctions in the 
     * order of appearence of the tiles in vc_tile_array
     */
    std::vector< boost::shared_ptr<tile_junction> > vc_junction_array;

    //! Tilecount in width direction
    long tilecount_width;
    //! Tilecount in height direction
    long tilecount_height;

    //! Width of the image in pixels
    long img_width;
    //! Height of the image in pixels
    long img_height;

    //! Width of the tile grid (every tile but the last has this width, the last may be smaller)
    long grid_width;
    //! Height of the tile grid (every tile but the last has this height, the last may be smaller)
    long grid_height;

public:
    /**
     * Standard constructor creates an empty tesselated image with
     * tilecount_width und tilecount_height equals zero.
     */
    tiled_image();

    /**
     * Create a tiled_image from an existing float_image. The tiled
     * image create a copy and thus not operate on the same memory as the
     * original float_image.
     * @param img The image to be split into tiles
     * @param tilecount_width_hint Number if tiles in width direction. This number is
     * not exactly adhered to, but the real number of tiles will be close.
     * @param tilecount_height_hint Number of tiles in height direction. This number is
     * not exactly adhered to, but the real number of tiles will be close.
     */
    tiled_image(sharedptr<row_major_float_image> img, long tilecount_width_hint, long tilecount_height_hint);


    /**
     * Frees the space associated with this image, if there are no more
     * strong pointers to the tiles within this image. Tiles will be
     * destroyed as soon as no strong pointers to them exist anymore.
     */
    virtual ~tiled_image();

    long get_tilecount_height() const;

    long get_tilecount_width() const;

    //!Returns width of the image in pixels
    long get_pixel_width() const;

    //!Returns height of the image in pixels
    long get_pixel_height() const;

    //returns size of junction array
    long get_size_of_junction_array() const;
    //returns size of tile array
    //    long get_size_of_tile_array() const;

    /**
     * Returns the junction on index i in the vc_junction_array for eg. iterating over 
     * @param i index of vc_junction_array
     * @return junction
     */
    boost::shared_ptr<tile_junction> get_junction_at(long i);

    /**
     * Returns the tile on index i in the vc_tile_array for eg. iterating over 
     * @param i index of vc_tile_array
     * @return tile
     */
    //    boost::shared_ptr<tile> get_tile_at(long i);


    //!Returns the tile at the specified index.
    boost::shared_ptr<tile> get_tile_at(long iw, long ih);

    /**
     * Create a junction for every junction between two first order neighbouring blocks
     * @return true if all junctions were created successfully, false if not or (!) junctions already initialized
     */
    bool create_all_junctions();

    /**
     * Overloaded function: You may use the other function with get_junction(tile t1, tile t2), too.
     * Returns the junction between t1 and the tile relative (in direction pos) to it, or unitizialed junction if there is no junction!
     * Note: Runtime is O(1)
     * @param t1 First tile
     * @param pos Second tiles position relative to first tile
     * @return Junction or unitialized junction if no junction exists
     */
    boost::shared_ptr<tile_junction> get_junction(boost::shared_ptr<tile> t, tile::rel_position pos);

    /**
     * Overloaded function: You may use the other function with get_junction(tile t, relative position), too
     * Returns the junction between t1 and t2, or unitizialed junction if there is no junction!
     * Note: Runtime is O(1)
     * @param t1 First tile
     * @param t2 Second tile
     * @return Junction between t1 and t2 or unitialized junction if no junction exists
     */
    boost::shared_ptr<tile_junction> get_junction(boost::shared_ptr<tile> t1, boost::shared_ptr<tile> t2);

    /**
     * Search for junctions belonging to the tile t
     * Note: Runtime is O(1)
     * @param t tile
     * @return vector of shared ptr to the junctions with tile t and other unspecified neighbour tile t2
     */
    boost::shared_ptr<std::vector<boost::shared_ptr<tile_junction> > > get_neighbouring_junctions(boost::shared_ptr<tile> t);
    /**
     * Search for junctions belonging to the tile t
     * Note: Runtime is O(1)
     * @param t tile
     * @param g tilegroup in which the common junctions with tile t are beeing searched
     * @return vector of shared ptr to the junctions with tile t and other unspecified neighbour tile t2 in group g
     */
    boost::shared_ptr<std::vector<boost::shared_ptr<tile_junction> > > get_neighbouring_junctions(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);

    /**
     * Search for first order (NSEW) neighbours of the given tile t. 
     * Note: Runtime is O(1)
     * @param t tile for which the neighbours are searched
     * @return Shared ptr to vector containing 0-4 weak ptr to the first order neighours
     */
    boost::shared_ptr<std::vector<boost::weak_ptr<tile> > > get_neighbours(boost::shared_ptr<tile> t);

    /**
     * Search for first order (NSEW) neighbours of the given tile t, but only (!) in the tilegroup g
     * NOTE: Neighbous of t NOT in the tilegroup g will not be added to the return statement!
     * Note: Runtime is O(1)
     * @param t tile for which the neighbours are searched
     * @param g tilegroup in which the neigbhours should be 
     * @return Shared ptr to vector containing 0-4 weak ptr to the frist order neighbours in the  tilegroup g
     */
    boost::shared_ptr<std::vector<boost::weak_ptr<tile> > > get_neighbours(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);


    /**
     * Generates a float image that contains a copy of the tiled_image.
     * @return A shared pointer to that image. This shared pointer has
     * a custom deleter with delete_float_image from float_image.h so that
     * it can deal with the memory-management without further need to address
     * it in the code.
     */
    boost::shared_ptr<row_major_float_image> convert_to_float_image();

    /**
     * This method will apply the unwrap method of the tile unwrapper
     * instance to every tile of the image. The order in which the tiles are
     * unwrapped is as follows (keep in mind, tile indices run from 0,...,W-1 in
     * width direction and 0,..., H-1 in height direction)
     * 
     * 1) First the rectangle with tiles w=0,...,W-2 and h=0,...,H-2 is unwrapped
     *    row by row.
     * 2) Then the column with w=W-1 is unwrapped (excluding bottom right tile with w=W-1, h=H-1)
     * 3) Lastly the bottom row is unwrapped (including bottom right tile)
     * 
     * This order makes sure that tiles that have the same dimensions will be 
     * unwrapped en bloque. For most unwrappers this will not make a difference.
     * However, if the unwrapper performs precalculations that only depend on
     * the dimensions of the tile (as is the case in the least squares gradient unwrapper),
     * this can significantly reduce the number of precalculations.
     * @param uw An instance of the unwrapper.
     */
    void unwrap_tiles(boost::shared_ptr<abstract_tile_unwrapper> uw);

    /**
     * This method calculates the gradient of the image by calculating the 
     * gradient of every tile.
     * @param dim The dimension (0 or 1) along which the gradient is to be calculated. 0 respresents width dimension and 1 represents height dimension. If a parameter other than 0,1 is given dim is set to 0 w/o warning.
     * @param gcalc Pointer to instance of gradient calculator. Standard argument is a second order gradient calculator on a unit cartesian system.
     */
    void gradient(int dim, sharedptr<abstract_gradient_calculator> gcalc = sharedptr<second_order_gradient>(new second_order_gradient())); //!todo exception

private:
    /**
     * This method performs the initialisation of the tiles with the values
     * from the given float_image and is called in the constructor. For the 
     * arguments see constructor.
     */
    void init_tiles(sharedptr<row_major_float_image> img, long tilecount_width_hint, long tilecount_height_hint);

    /**
     * Create a junction between tile at index i in the private vc_tile_array and the direct neighbour in
     * the direction of pos
     * This method DOES check wether the tile and position are legal (eg. tiles on left edge have no left neighbour).
     * This method DOES NOT check wether this specific junction is initialized -> Use create_junctions instead of this method!
     * @param i index for the vc_tile_array
     * @param pos position of neighbour
     * @return shared ptr to junction OR boost::none (if no junction can be returned)
     */
    boost::optional<boost::shared_ptr<tile_junction> > create_junction(long i, tile::rel_position pos);

    //Can be removed: 
    void test_all_junctions_test();
};

#endif	/* SMART_TILED_IMAGE_H */

