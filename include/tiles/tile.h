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
 * File:   tile.h
 * Author: g.antonopoulos, b.steltner
 *
 * Created on 15. Oktober 2014, 16:45
 */

#ifndef TILE_H
#define	TILE_H

//#include <vector>
#include "tiles/tilegroup.h"

//FORWARD DECLARATION START
class abstract_coordinate_system;
class abstract_function;
class abstract_gradient_calculator;
class tile_junction;
class tile;
//FORWARD DECLARATION END

/**
 * This file offers the functionality of the tile class while using boost
 * smart pointers to make memory management easier.
 * One MAJOR DIFFERENCE is that it creates its own copy of the data and
 * does not operate on pointers to the float image data.
 * 
 */

/**
 * Sets the tilegroup of the tile t to the given group and adds this
 * tile to the vector of tiles in the tilegroup g. ACHTUNG! This method should
 * only be used if this tile is not alreay member in a tilegroup. If
 * it is member of a tilegroup, then the reference to this tile in the old
 * group will still be there. This can lead to errors. For merging tilegroups
 * use the merge_into method from tilegroup.
 * These conditions are not checked.
 * @param t the tile. It must not be in a group.
 * @param g the tilegroup. It must not already contain the tile t.
 */
void add_tile_to_group(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);
/**
 * Merges tilegroup g1 into group g2. All tiles from g1 will now
 * point to g2 as their group and g1 will be devoid of elements. 
 * @param g1 
 * @param g2
 */
void merge_tilegroups(boost::shared_ptr<tilegroup> g1, boost::shared_ptr<tilegroup> g2);

class tile {
    //    friend class tiled_image;
    friend class tilegroup;
    friend void add_tile_to_group(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);
    friend void merge_tilegroups(boost::shared_ptr<tilegroup> g1, boost::shared_ptr<tilegroup> g2);

    /**
     * This enum provides a way of describing the relative position of two tiles.
     * The values are chosen pretty much arbitrarily but such that -UP = DOWN and
     * -RIGHT = LEFT.
     * Meaning: right/left =  +/- 1 in w direction and down/up = +/- 1 in h direction
     * 
     * The tile junction does not own the tiles, that means it has only weak pointers
     * to the tiles.
     */
public:

    enum rel_position {
        UP = 123, DOWN = -123, RIGHT = 321, LEFT = -321
    };

private:
    /**
     * This is a shared pointer that will point to the memory, where the
     * data is located. Copies of the tile will have a shared pointer
     * to the same memory.
     */
    boost::shared_array<float> sa_data; //initialisiert mit Nullpointer

    //!Strong pointer to the tilegroup that this tile is in 
    //Only if all tiles of a tilegroup are deleted, the tilegroup itself will be deleted
    boost::weak_ptr<tilegroup> ptr_tilegroup;

    /* 
     * A smart tile has 4 weak ptrs to its belonging junctions. These pointers are initialized
     * via the set_junctions-method, only when the junction is created via its factory method.
     * If there is no junction, these ptrs stay uninitiziliased and have to checked via weak_ptr::expired()
     */
    //    boost::weak_ptr<tile_junction> ptr_junc_up;
    //    boost::weak_ptr<tile_junction> ptr_junc_down;
    //    boost::weak_ptr<tile_junction> ptr_junc_left;
    //    boost::weak_ptr<tile_junction> ptr_junc_right;


    //! Actual width and height of the tile.
    long width, height;
    //! Actual position of this tile in the tilegroup
    long posH, posW;

    /**
     * Whenever a value is added to this tile, this value will be increased instead of increasing
     * every value of the tiles pixels. Whenever a pixel needs to return its value, this value will 
     * be added to it via its own get-method.
     */
    float tile_offset;


public:
    /**
     * This constructor performs nothing and reserves no memory for data.
     * The width and heigt of the tile will be 0 (zero). See also operator=.
     */
    tile();

    /**
     * Constructor with explicit values at initialisation
     * @param values sharedarray of width*height float-values
     * @param width width of the tile
     * @param height height of the tile
     * @param posH position in the grid
     * @param posW position in the grid
     */
    tile(sharedarray<float> values, long width, long height, long posH, long posW);

    //! Destructor
    ~tile();

    /**
     * Copy Constructor. The new tile will point to the same memory as the old
     * tile.
     * @param other Given tile.
     * @return 
     */
    tile(const tile& other);

    /**
     * Assignment operator. This operator assigns the properties and elements of
     * other to this. This tile will then share ownership of the data array of
     * the other tile. 
     * 
     * The operator will throw a tile_dimension_exception if this tile and the
     * other tile have different dimensions. This prevents the dimensions of a 
     * tiled_image changing when assigning something to a tile. 
     * Only when this tile has both dimensions equals 0 (zero) nothing will be thrown.
     * 
     * @throws tile_dimension_exception Thrown when this tile has dimensions other
     * than zero which are unequal to the other tiles dimension.
     * @param other
     * @return Reference to *this.
     */
    tile& operator=(const tile & other) /* throw(tile_dimension_exception) */;

    /**
     * Dieser Konstruktor erstellt eine neue Tile, die eine lokale Kopie der
     * Daten auf den entsprechenden Bereich aus einem float_image erstellt. Dabei wird das 
     * Bild in ein Gitter mit grid_width und grid_height zerteilt und die Tiles
     * entsprechend indiziert (w:links->rechts, h:oben nach unten, wobei linke obere
     * Ecke iw,ih=0,0 entspricht). Falls das Gridding genau aufgeht sind alle Tiles
     * gleich groß nämlich grid_width*grid_height. Falls nicht, dann werden die 
     * untersten Tiles entsprechend kleiner gemacht.
     * Vergleiche Methode 
     * @param img Das Bild, welches in Tiles zerlegt werden soll
     * @param grid_width Die (maximale) Anzahl der Elemente in dem Tile.
     * @param grid_height
     * @param iw Index der Tile in w-Richtung
     * @param ih 
     */
    tile(sharedptr<row_major_float_image> img, long grid_width, long grid_height, long iw, long ih);

    //!Get value of pixel in tile (raw_value + offset)
    float get_value_at(long iw, long ih);

    //!Get the raw value of the pixel in this tile, explicitly without the tile-offset
    float get_raw_value_at(long iw, long ih);

    //!Set value of the specified pixel to val
    void set_value_at(long iw, long ih, float val);

    //!Add value of the specified pixel
    void add_value_at(long iw, long ih, float val);
    //!Get offset of this tile
    float get_offset();
    //!Get height number of elements in tile in h-direction
    long get_height() const;

    //!Get number of elements in tile in w-direction
    long get_width() const;

    //!Adds a specified value to each pixel in the tile. Returns *this.
    tile & add_value(float val);

    /**
     * Set the pixel values of the tile to the given function values, evaluated on the given coordinate system.
     * @param func Function
     * @param coordsys Coordinate system
     * @return reference to this.
     */
    tile & set_values(sharedptr<abstract_function> func, sharedptr<abstract_coordinate_system> coordsys);

    /**
     * Apply the tile-offset and  to the values of the pixels, 
     * resetting the tile- and tilegroup-offset.
     */
    //!!NOT NEEDED AND POSSIBLY DANGEROUS IF TILEGROUP GETS OWN OFFSET
    //void apply_value();

    //! Wraps all values in this tile to interval [-Pi, Pi]. Returns pointer to this*.
    tile & wrap();

    /**
     * For each pixel T sets pixel to T= wrap(T+val)-val.
     * @param val value to add.
     * @return pointer to this
     */
    tile & rewrap(float val);

    /**
     * Calculates the mean value of the tile's pixels.
     * @return mean value
     */
    float calc_mean();

    /**
     * @return true if this tile is member in a tilegroup, false otherwise.
     */
    bool has_tilegroup();

    //!Get position in h-direction of this tile in the tilegroup
    long get_posH() const;

    //!Get position in w-direction of this tile in the tilegroup
    long get_posW() const;

    /**
     * Check other & this tile if they are first order neighbours
     * @param other tile 
     * @return true, if other & this are first order neighbours
     */
    bool is_neighbour(boost::shared_ptr<tile> other);

    /**
     * 
     * @return Pointer to the tilegroup. If no tilegroup is present a NULL pointer
     * is returned. That may lead to bad things... so check first that the tile
     * has a group.
     */
    boost::shared_ptr<tilegroup> get_tilegroup();

    /**
     * Return the junction in direction pos.
     * @param pos position
     * @return (un-/initialized) weak ptr to junction. Depends on if a junction is existing in his direction
     */
    //    boost::shared_ptr<tile_junction> get_junction(tile::rel_position pos);

    //private:
    /**
     * Sets the matching pointer to the junction. 
     * Note: The relative position of the tile to the junction will be evaluated inside this method.
     * 
     * @param junc Junction
     */
    //    void set_junction(boost::weak_ptr<tile_junction> junc);


};

#endif	/* TILE_H */

