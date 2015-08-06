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
 * File:   tile_junction.h
 * Author: g.antonopoulos, b.steltner
 *
 * Created on 29. Oktober 2014, 08:44
 */

#ifndef TILE_JUNCTION_H
#define	TILE_JUNCTION_H

#include "tiles/tile.h"

/**
 *  A little helper that encapsules a connection between two
 * tiles. It stores the two tiles connected as well as the relative position of
 * the second tile to the first. It also provides methods for calculating the
 * differences, variance of the junction.
 */
class tile_junction {
    friend class tiled_image;


private:
    /**
     * Hidden std constructor
     */
    tile_junction();

    /**
     * Hidden constructor for a new junction. Since it is private, it may only be instantiated through the friend class tiled_image 
     * @param first First tile
     * @param second Second tile
     * @param second_to_first Position of the second tile relative to first tile 
     */
    tile_junction(boost::weak_ptr<tile> first, boost::weak_ptr<tile> second, tile::rel_position second_to_first);
    //!The first tile of the junction
    boost::weak_ptr<tile> first;
    //!The second tile of the junction
    boost::weak_ptr<tile> second;
    //!Position of the second tile relative to first tile (right/left = w +/- 1 and down/up = h +/- 1)
    tile::rel_position second_to_first;

    /* This value is used to save the variance of the junction. */
    float variance;
    /* Boolean if variance_float has been calculated already. True: Is calculated. False: Is not calculated */
    bool has_variance_calculated;
    /* This value is used to save the mean of the junction.*/
    float mean;
    /* Boolean if variance_float has been calculated already. True: Is calculated. False: Is not calculated */
    bool has_mean_calculated;

    /*
    [t1]
    [t2] -> UP

    [t2]
    [t1] -> DOWN

    [t1][t2] ->RIGHT
    [t2][t1] ->LEFT
     */

    /**
     * Factory Method: Constructs a new junction between two smart tiles.
     * The constructor is private so that a new junction can only be
     * constructed using the static create_new method. Since this method is private, too, 
     * a junction may only be constructed via the friend class tiled_image. 
     * This guarantees that junctions are solely owned by smart tiled images.
     * In addition the tiles get a weak ptr reference to the junction for faster access.
     * @param first weak_ptr to first tile
     * @param second weak_ptr to second tile
     * @param second_to_first Position of the second tile relativ to first tile
     * @return shared_ptr to the newly created junction
     */
    static boost::shared_ptr<tile_junction> create_new(boost::weak_ptr<tile> first, boost::weak_ptr<tile> second, tile::rel_position second_to_first) {
        boost::shared_ptr<tile_junction> junction(new tile_junction(first, second, second_to_first));
        junction->has_variance_calculated = false;
        junction->has_mean_calculated = false;
        return junction;
    }
public:

    ~tile_junction();

    boost::weak_ptr<tile> get_first();

    boost::weak_ptr<tile> get_second();

    /**
     * Gives the relative position of second tile to first tile.
     * @return the relative position
     */
    tile::rel_position get_relative_position();
    /**
     * Calculates the mean difference of the junction between the two tiles this junction inherits
     * @return mean difference value mostly between [-3 Pi, 3 Pi]
     */
    float calc_junction_mean_difference();

    /**
     * Returns mean_difference. This method DOES NOT check if it is calculated/initialised.
     * @return mean_difference of this junction.
     */
    float get_mean();

    /**
     * Is the mean calculated? 
     * @return False if mean is not initialised. True if it is calculated
     */
    bool has_mean();

    /**
     * Calculates the variance  of the junction between the two tiles this junction inherits
     * @return variance value between [0,small float], mostly only up to ~0.001
     */
    float calc_junction_variance();

    /**
     * Returns the variance. This method DOES NOT check if it is calculated/initiliased.
     * @return 
     */
    float get_variance();

    /**
     * Is the variance calculated?
     * @return False if variance is not initialised. True if it is calculated
     */
    bool has_variance();

    /**
     * Calculates the squared difference of the junction between the two tiles this junction inherits
     * @return squared difference
     */
    float calc_junction_squared_difference();
    /**
     * Check if this junction is a junction between the tile t1 and t2 (not necessarily in this order!).
     * @param t1 One tile (may be the second tile!)
     * @param t2 Another tile (may be the first tile!)
     * @return true, if this junction is between t1 and t2
     */
    bool is_junction_between(boost::shared_ptr<tile> t1, boost::shared_ptr<tile> t2);

    /**
     * Check if this junction is a junction between the tile t and an unspecified tile t2 in the direction "pos".
     * IMPORTANT: t may be the first OR the second tile. If t is the second tile however, the relative position gets "flipped" by "-1" (eg. DOWN => UP).
     * EXAMPLE #1: Let t be the FIRST tile t1, pos be "RIGHT" & we call the second tile "other":
     * This function shall return TRUE, IFF "other" is on the RIGHT-hand side of t ( <=> t2 is on RIGHT-hand side of t1 <=> [t1][t2] !)
     * EXAMPLE #2: Let t be the SECOND tile t2, pos be "RIGHT" & we call the first tile "other":
     * This function shall return TRUE, IFF t is on the RIGHT-hand side of "other" ( <=> t2 is on RIGHT-hand side of t1 <=> [t1][t2] )
     * NOTE: Both statements are equivalent: [ other is on the RIGHT-hand side of t ] <=> [ t is on the LEFT-hand side of other ]
     *
     * Further NOTE: With the flip of the relative position, dependent on t as the first or the second tile, EACH pair (t, pos) has an UNIQUE junction it belongs to.
     * Therefore only one junction can return true with a given tile t and a relative position pos, making it safe to search through eg. a list of junctions to find 
     * the belonging junction.
     * @param t The stationary tile
     * @param pos The relative position in which the OTHER tile is to t (NOT t1 is to t2!)
     * @return true, if this junction belongs to t and the relative position matches.
     */
    bool is_junction_between(boost::shared_ptr<tile> t, tile::rel_position pos);


    /**
     * Write informations about this class into the standard output
     */
    void toString();
};


#endif	/* TILE_JUNCTION_H */

