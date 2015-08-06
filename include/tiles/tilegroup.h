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
 * File:   tilegroup.h
 * Author: b.steltner, g.antonopoulos
 *
 * Created on 14. November 2014, 11:57
 */

#ifndef TILEGROUP_H
#define	TILEGROUP_H

#include "basic_includes.h"
#include "image/row_major_float_image.h"
#include <forward_list>

class tile; //Forward declaration of class tile. They are referencing each other!

/**
 * @class tilegroup Eine Klasse, die eine Gruppe von Smart-Tiles verwaltet.
 * Die tiles haben strong pointer auf ihre tilegroup, wohingegen diese tilegroup
 * nur schwache pointer auf die tiles haben. Die starken pointer auf die tiles
 * sitzen im tiled_image.
 * Die tilegroup Objekte müssen demnach nirgendwo zwischengespeichert
 * werden. Sie können über die entsprechenden tiles angesprochen werden. Wenn
 * keine tile mehr auf die tilegroup zeigt, wird diese automatisch zerstört.
 * 
 * Die Funktion zum hinzufügen von tiles zu einer Gruppe befindet sich in der
 * tile Klasse.
 */
class tilegroup {
    friend class tiled_image;
    friend class tile;
    friend void add_tile_to_group(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);
    friend void merge_tilegroups(boost::shared_ptr<tilegroup> g1, boost::shared_ptr<tilegroup> g2);
public:
    //! Destruktor
    ~tilegroup();
    /**
     * NOTE: This operation has O(n)! DO NOT USE this if possible
     * @return The number of elements in the tilegroup.
     */
    int size();
    /**
     * Add value to all tiles in the group.
     * @param val The value to add.
     */
    void add_value(float val);

    /**
     * Static creator method, so that new tilegoups can only be constructed
     * like this 
     * @return A strong pointer to a new tilegroup.
     */
    static boost::shared_ptr<tilegroup> create_new() {
        boost::shared_ptr<tilegroup> p(new tilegroup());
        return p;
    }
    /**
     * Adds a tile to an existing tilegroup
     * Note: The pointer g has to be a paramter, so that the tile t may be given the 
     * pointer to this group. ( *this is not a valid shared pointer!)
     * @param t tile to add
     * @param g tilegroup for adding the tile
     */ 
    void add_tile_to_group(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g);
    
    /**
     * Adds a tile to this group, iff it is not included anyway.
     * Note: The pointer g has to be a paramter, so that the tile t may be given the 
     * pointer to this group. ( *this is not a valid shared pointer!)
     * @param t tile to add
     * @return true if tile has been added. False if tile was included in the group anyway
     */
    bool add_unique_tile(sharedptr<tile> t, sharedptr<tilegroup> g);
    

    /**
     * Merges two tilegroups together. 
     * @param g1 smaller tilegroup
     * @param g2 bigger tilegroup
     */
    void merge_tilegroups(boost::shared_ptr<tilegroup> g1, boost::shared_ptr<tilegroup> g2);
    
private:
    /**
     * Forward List for the weak ptr to the smart tiles. 
     */
    std::forward_list<boost::weak_ptr<tile> > fl_ptr_tiles;

    /**
     * Constructs a new tilegoup without elements.
     * The constructor is private so that a new tilegroup can only be
     * constructed using the static create_new method.
     */
    tilegroup();

};

#endif	/* TILEGROUP_H */

