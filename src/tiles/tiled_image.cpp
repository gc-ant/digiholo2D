#include "tiles/tiled_image.h"

#include "tiles/tile_junction.h"
#include "algorithm/abstract_tile_unwrapper.h"
#include "algorithm/abstract_gradient_calculator.h"

tiled_image::tiled_image() : tilecount_width(0), tilecount_height(0), img_width(0), img_height(0), grid_width(0), grid_height(0) { //empty
}

tiled_image::tiled_image(sharedptr<row_major_float_image> img, long tilecount_width_hint, long tilecount_height_hint) {
    this->img_width = img->get_width();
    this->img_height = img->get_height();
    this->init_tiles(img, tilecount_width_hint, tilecount_height_hint);
}

tiled_image::~tiled_image() {
    //   this->vc_tile_array.clear();   //change by b.steltner
    //   this->vc_junction_array.clear();  //clear destroys all elements in the vector, but a reallocation is not guaranteed to happen.
    //   std::vector<sharedptr<tile> >().swap(this->vc_tile_array); //create a new vector w/o data and replace it with vc_tile_array => the latter one is destroyed
    //   std::vector<sharedptr<tile_junction> >().swap(this->vc_junction_array);
}

long tiled_image::get_tilecount_height() const {
    return this->tilecount_height;
}

long tiled_image::get_tilecount_width() const {
    return this->tilecount_width;
}

long tiled_image::get_pixel_width() const {
    return this->img_width;
}

long tiled_image::get_pixel_height() const {
    return this->img_height;
}

long tiled_image::get_size_of_junction_array() const {
    return this->vc_junction_array.size();
}
//long tiled_image::get_size_of_tile_array() const {
//   return this->vc_tile_array.size();
//}

boost::shared_ptr<tile_junction> tiled_image::get_junction_at(long i) {
    return this->vc_junction_array.at(i);
}
//boost::shared_ptr<tile> tiled_image::get_tile_at(long i)      {
//   return this->vc_tile_array.at(i);
//}

boost::optional<boost::shared_ptr<tile_junction> > tiled_image::create_junction(long i, tile::rel_position pos) {
    long W = this->tilecount_width;
    long H = this->tilecount_height;
    boost::weak_ptr<tile> t1(this->vc_tile_array.at(i)); //"stationary" tile. Second tile will be evaluated with respect to the relative position "pos"

    switch (pos) {
        case tile::UP:
        {
            if (i > (W - 1)) { //eg. W = 6 => tiles 0,1,2,3,4,5 have no neighbours on top
                boost::weak_ptr<tile> t2(this->vc_tile_array.at(i - W));
                boost::shared_ptr<tile_junction> junction = tile_junction::create_new(t1, t2, tile::UP);

                return junction;
            } else {
                /* These error messages are not displayed, because the call for this method may be without further check if a junction should be created here */
                //            PRINTLN("Error in createJunction: UP");
            }
            break;
        }
        case tile::DOWN:
        {
            if (i < W * (H - 1)) { //eg: W = 6 & H = 3 => tiles 12,13,14,15,16,17  have no neighbours underneath
                boost::weak_ptr<tile> t2(this->vc_tile_array.at(i + W));
                boost::shared_ptr<tile_junction> junction = tile_junction::create_new(t1, t2, tile::DOWN);
                return junction;
            } else {
                //            PRINTLN("Error in createJunction: DOWN");
            }
            break;
        }
        case tile::LEFT:
        {
            if (!(i % (W) == 0)) { //eg. W = 6 => tiles 0, 6, 12, 18 have no left neighbours
                boost::weak_ptr<tile> t2(this->vc_tile_array.at(i - 1));
                boost::shared_ptr<tile_junction> junction = tile_junction::create_new(t1, t2, tile::LEFT);
                return junction;
            } else {
                //            PRINTLN("Error in createJunction: LEFT");
            }
            break;
        }
        case tile::RIGHT:
        {
            if (!((i + 1) % W == 0)) { //eg. W = 6 => tiles 5, 11, 17, 23 have no left neighbours
                boost::weak_ptr<tile> t2(this->vc_tile_array.at(i + 1));
                boost::shared_ptr<tile_junction> junction = tile_junction::create_new(t1, t2, tile::RIGHT);
                return junction;
            } else {
                //            PRINTLN("Error in createJunction: RIGHT");
            }
            break;
        }
        default:
        {
            PRINTLN("Error in createJunction: No legal position comitted");
        }
    }
    return boost::none;
}

bool tiled_image::create_all_junctions() {
    //   PRINTLN("Creating all junctions via method image::create_all_junctions");
    if (this->vc_junction_array.size() != 0) { //vector is initialized
        return false; //do nothing and return false: nothing was done here
    } else { //initialize every junction
        //iterate over every smart tile in vc_tile_array
        //        PRINTLN("Size of tile array: " << vc_tile_array.size());
        boost::optional<boost::shared_ptr<tile_junction> > junction;

        for (std::vector<boost::shared_ptr<tile> >::size_type i = 0; i < this->vc_tile_array.size(); i++) {
            junction = create_junction((long) i, tile::DOWN);
            if (!(junction == boost::none)) {
                this->vc_junction_array.push_back(*junction);
            }
        }
        for (std::vector<boost::shared_ptr<tile> >::size_type i = 0; i < this->vc_tile_array.size(); i++) {
            junction = create_junction((long) i, tile::RIGHT);
            if (!(junction == boost::none)) {
                this->vc_junction_array.push_back(*junction);
            }
        }
        /*
         * We only add junctions in the direction DOWN and RIGHT, because we iterate over every tile, eg.: 
         * First we are at tile t1 and create the junction to the right to t2. Then next step we are at tile t2 and 
         * create a junction left to t1. Now we would have 2 junctions. To prevent this, only add DOWN and RIGHT...
         * 
         * Note: create_junction(i,pos) checks if there is a tile in the direction...
         */
    }
    //    PRINTLN("Size of junction array: " << vc_junction_array.size());

    //   this->test_all_junctions_test();

    return true;
}

boost::shared_ptr<tile_junction> tiled_image::get_junction(boost::shared_ptr<tile> t, tile::rel_position pos) {
    /* Debug prints are disabled, because get_neighbouring_junctions is calling this method with "wrong parameters" often...*/
    if (this->vc_junction_array.size() == 0) {
        this->create_all_junctions();
        return this->get_junction(t, pos);
        PRINTLN("Error in tiled_image::get_junction (1 tile, 1 position): Vector vc_junction empty! No Junction initialized");
    } else {
        int i = t->get_posH() * this->tilecount_width + t->get_posW(); //get the index of the tile in the vc_tile_array
        switch (pos) {
            case tile::UP:
            {
                if (i >= this->tilecount_width) {
                    boost::shared_ptr<tile_junction> junction = vc_junction_array.at(i - this->tilecount_width);
                    return junction;
                } else {
                    //               PRINTLN("Error in tiled_image::get_junction(tile, position): No junction up at index: "<<i)
                }
                break;
            }
            case tile::DOWN:
            {
                if (i < (this->tilecount_height - 1) * this->tilecount_width) {
                    boost::shared_ptr<tile_junction> junction = vc_junction_array.at(i);
                    return junction;
                } else {
                    //               PRINTLN("Error in tiled_image::get_junction(tile, position): No junction down at index: "<<i)
                }
                break;
            }
            case tile::RIGHT:
            {
                /* First width*(height-1) junctions are DOWN only, i = width*(height-1) is first junction RIGHT
                 * In every row is a "missing" junction (because no LEFT/RIGHT neighbour. So for the row t is in, 
                 * we have to substract 1 "missing" junction each row before */
                if (!((t->get_posW() + 1) % this->tilecount_width == 0)) {
                    long index = this->tilecount_width * (this->tilecount_height - 1) + i - t->get_posH();
                    boost::shared_ptr<tile_junction> junction = vc_junction_array.at(index);
                    return junction;
                } else {
                    //               PRINTLN("Error in tiled_image::get_junction(tile, position): No junction right at index: "<< this->tilecount_width * (this->tilecount_height - 1) + i - t->get_posH())
                }
                break;
            }
            case tile::LEFT:
            {
                if (!(t->get_posW() % this->tilecount_width == 0)) {
                    //note: as case RIGHT, but index -1, because the lef junction of tile i is the right junction of tile i-1
                    long index = this->tilecount_width * (this->tilecount_height - 1) + i - t->get_posH() - 1;
                    boost::shared_ptr<tile_junction> junction = vc_junction_array.at(index);
                    return junction;
                } else {
                    //               PRINTLN("Error in tiled_image::get_junction(tile, position): No junction left"<< this->tilecount_width * (this->tilecount_height - 1) + i - t->get_posH())
                }
                break;
            }
        }
    }
    //   PRINTLN("Error in tiled_image::get_junction (1 tile, 1 position): Returning non-initizalied Junction.");
    boost::shared_ptr<tile_junction> junctionNULL;
    return junctionNULL;
}

bool help_is_junction(boost::shared_ptr<tile_junction> junc, boost::shared_ptr<tile> t, tile::rel_position pos) {
    if (junc) { //is initialized?
        if (junc->get_first().lock() == t || junc->get_second().lock() == t) {//t is part of the junction?
            return true;
        }
    }
    return false;
}

boost::shared_ptr<tile_junction> tiled_image::get_junction(boost::shared_ptr<tile> t1, boost::shared_ptr<tile> t2) {
    if (this->vc_junction_array.size() == 0) {
        this->create_all_junctions();
        return this->get_junction(t1, t2);
        PRINTLN("Error in tiled_image::get_junction (2 tiles): Vector vc_junction empty! No Junction initialized");
    } else {
        boost::shared_ptr<tile_junction> compare = this->get_junction(t1, tile::UP);
        //test if junction is initialized and if t2 is (the other) part of the junction
        if (help_is_junction(compare, t2, tile::UP)) {
            return compare;
        }
        compare = this->get_junction(t1, tile::DOWN);
        if (help_is_junction(compare, t2, tile::DOWN)) {
            return compare;
        }
        compare = this->get_junction(t1, tile::LEFT);
        if (help_is_junction(compare, t2, tile::LEFT)) {
            return compare;
        }
        compare = this->get_junction(t1, tile::RIGHT);
        if (help_is_junction(compare, t2, tile::RIGHT)) {
            return compare;
        }
        PRINTLN("Error in tiled_image::get_junction: No junction found for tile at iw:" << t1->get_posW() << " ih:" << t1->get_posH() << " and tile at iw:" << t2->get_posW() << " ih:" << t2->get_posH());
    }
    boost::shared_ptr<tile_junction> junctionNULL;
    return junctionNULL;
}

boost::shared_ptr<std::vector<boost::shared_ptr<tile_junction> > > tiled_image::get_neighbouring_junctions(boost::shared_ptr<tile> t) {
    boost::shared_ptr<std::vector<boost::shared_ptr<tile_junction> > > ruckgabe(new std::vector<boost::shared_ptr<tile_junction> >);
    if (this->vc_junction_array.size() == 0) {
        this->create_all_junctions();
        return this->get_neighbouring_junctions(t);
        PRINTLN("Error in tiled_image::get_neighbouring_junction (1 tile): Vector vc_junction empty! No Junction initialized");
    } else {
        boost::shared_ptr<tile_junction> junction;
        //Note: For optimisation check if t has a legit junction in the direction up/down/left/right
        //Note: Operator = (NOT ==) => With boost: Assign junction & check if initialized
        if (junction = this->get_junction(t, tile::UP)) {
            ruckgabe->push_back(junction);
        }
        if (junction = this->get_junction(t, tile::DOWN)) {
            ruckgabe->push_back(junction);
        }
        if (junction = this->get_junction(t, tile::LEFT)) {
            ruckgabe->push_back(junction);
        }
        if (junction = this->get_junction(t, tile::RIGHT)) {
            ruckgabe->push_back(junction);
        }
    }
    return ruckgabe;
}

/**
 * Local help function for method get_neighbouring_junctions(tile t, tilegroup g); 
 * @param junc junction
 * @param t tile
 * @param g tilegroup in which the other unspecified tile has to be in to retun true
 * @return true if the other tile of the junction with t is in the tilegroup g
 */
bool help_is_in_tilegroup(boost::shared_ptr<tile_junction> junc, boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g) {
    if (junc) {
        /*             t is t1          AND                tilegroup of t2 is g              */
        if ((junc->get_first().lock() == t) && (junc->get_second().lock()->get_tilegroup() == g)) {
            return true;
            /*             t is t2          AND                tilegroup of t1 is g              */
        } else if ((junc->get_second().lock() == t) && (junc->get_first().lock()->get_tilegroup() == g)) {
            return true;
        }
    }
    return false;
}

boost::shared_ptr<std::vector<boost::shared_ptr<tile_junction> > > tiled_image::get_neighbouring_junctions(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g) {
    boost::shared_ptr<std::vector<boost::shared_ptr<tile_junction> > > ruckgabe(new std::vector<boost::shared_ptr<tile_junction> >);
    if (this->vc_junction_array.size() == 0) {
        this->create_all_junctions();
        return this->get_neighbouring_junctions(t, g);
        PRINTLN("Error in tiled_image::get_neighbouring_junction (1 tile, 1 tilegroup): Vector vc_junction empty! No Junction initialized");
    } else {
        boost::shared_ptr<tile_junction> junction;
        junction = this->get_junction(t, tile::UP);
        //Check if junction is initialized and the other tile in the junction with t is in the tilegroup g
        if (help_is_in_tilegroup(junction, t, g)) {
            ruckgabe->push_back(junction);
        }
        junction = this->get_junction(t, tile::DOWN);
        if (help_is_in_tilegroup(junction, t, g)) {
            ruckgabe->push_back(junction);
        }
        junction = this->get_junction(t, tile::LEFT);
        if (help_is_in_tilegroup(junction, t, g)) {
            ruckgabe->push_back(junction);
        }
        junction = this->get_junction(t, tile::RIGHT);
        if (help_is_in_tilegroup(junction, t, g)) {
            ruckgabe->push_back(junction);
        }
    }
    if (ruckgabe->size() == 0) {
        PRINTLN("Error in image::get_neighbouring_junctions(tile,group): Return has no elements");
    }
    return ruckgabe;
}

boost::shared_ptr<std::vector<boost::weak_ptr<tile> > > tiled_image::get_neighbours(boost::shared_ptr<tile> t) {
    boost::shared_ptr<std::vector<boost::weak_ptr<tile> > > ruckgabe(new std::vector<boost::weak_ptr<tile> >);
    long tw = t->get_posW();
    long th = t->get_posH();
    if (tw > 0) {
        boost::weak_ptr<tile> takeLeft = this->vc_tile_array.at(tw - 1);
        ruckgabe->push_back(takeLeft);
    }
    if (tw < t->get_width() - 1) {
        boost::weak_ptr<tile> takeRight = this->vc_tile_array.at(tw + 1);
        ruckgabe->push_back(takeRight);
    }
    if (th > 0) {
        boost::weak_ptr<tile> takeDown = this->vc_tile_array.at(th * (t->get_width() + 1) + tw);
        ruckgabe->push_back(takeDown);
    }
    if (th < t->get_height() - 1) {
        boost::weak_ptr<tile> takeUp = this->vc_tile_array.at(th * (t->get_width() - 1) + tw);
        ruckgabe->push_back(takeUp);
    }
    return ruckgabe;
}

boost::shared_ptr<std::vector<boost::weak_ptr<tile> > > tiled_image::get_neighbours(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g) {
    boost::shared_ptr<std::vector<boost::weak_ptr<tile> > > ruckgabe(new std::vector<boost::weak_ptr<tile> >);
    long tw = t->get_posW();
    long th = t->get_posH();

    if (tw > 0) {
        boost::shared_ptr<tile> takeLeft = this->vc_tile_array.at(tw - 1);
        if (takeLeft->get_tilegroup() == g) {
            boost::weak_ptr<tile> left = takeLeft;
            ruckgabe->push_back(left);
        }
    }
    if (tw < t->get_width() - 1) {
        boost::shared_ptr<tile> takeRight = this->vc_tile_array.at(tw + 1);
        if (takeRight->get_tilegroup() == g) {
            boost::weak_ptr<tile> right = takeRight;
            ruckgabe->push_back(right);
        }
    }
    if (th > 0) {
        boost::shared_ptr<tile> takeDown = this->vc_tile_array.at(th * (t->get_width() + 1) + tw);
        if (takeDown->get_tilegroup() == g) {
            boost::weak_ptr<tile> down = takeDown;
            ruckgabe->push_back(down);
        }
    }
    if (th < t->get_height() - 1) {
        boost::shared_ptr<tile> takeUp = this->vc_tile_array.at(th * (t->get_width() - 1) + tw);
        if (takeUp->get_tilegroup() == g) {
            boost::weak_ptr<tile> up = takeUp;
            ruckgabe->push_back(up);
        }
    }
    return ruckgabe;
}

boost::shared_ptr<tile> tiled_image::get_tile_at(long iw, long ih) {
    if (iw >= 0 && ih >= 0 && iw < tilecount_width && ih < tilecount_height) {
        boost::shared_ptr<tile> sp = this->vc_tile_array.at(iw + ih * tilecount_width);
        return sp;
    } else {
        //leeren Pointer zurückgeben, falls die Tile nicht existiert.
        boost::shared_ptr<tile> sp;
        return sp;
    }
}

boost::shared_ptr<row_major_float_image> tiled_image::convert_to_float_image() {
    //!Shared pointer mit custom deleter übergeben, damit das Memory Management übernommen werden kann.
    boost::shared_ptr<row_major_float_image> fip(new row_major_float_image(this->img_width, this->img_height), delete_float_image);

    //Jetzt die tiles einzeln durchgehen und daraus das Bild zusammenstellen.
    //Dazu die tile indices in w und h Richtung, itw bzw ith abarbeiten

    boost::shared_ptr<tile> cur_tile;
    float cur_val;

    long twidth, theight;

    for (long itw = 0; itw < this->tilecount_width; itw++) {
        for (int ith = 0; ith < this->tilecount_height; ith++) {
            //Jetzt innerhab der tile die Pixel-Positionen iw, ih durchfahren
            cur_tile = this->get_tile_at(itw, ith);
            twidth = cur_tile->get_width();
            theight = cur_tile->get_height();
            for (long iw = 0; iw < twidth; iw++) {
                for (long ih = 0; ih < theight; ih++) {
                    cur_val = cur_tile->get_value_at(iw, ih);
                    fip->set_pixel(itw * this->grid_width + iw, ith * this->grid_height + ih, cur_val);
                }
            }
        }
    }

    return fip;
}

void tiled_image::init_tiles(sharedptr<row_major_float_image> img, long tilecount_width_hint, long tilecount_height_hint) {
    if (tilecount_width_hint == 0 || tilecount_width_hint == 0)
    {
        this->tilecount_width = 0;
        this->tilecount_height = 0;
        return;
    }

    long max_tilewidth = img->get_width() / tilecount_width_hint;       //long, therefore decimal is chopped off
    long max_tileheight = img->get_height() / tilecount_height_hint;    //calculate maximum pixel of 1 block (rounded down)

    //Anzahl tiles neu berechnen, so dass die ürsprünglich gemeinte Breite übernommen wird. 
    this->tilecount_width = img->get_width() / max_tilewidth;       //long, therefore decimal is chopped of, again. 
    this->tilecount_height = img->get_height() / max_tileheight;

    this->grid_width = max_tilewidth;
    this->grid_height = max_tileheight;

    //Dies ist damit das Bild auch mit den Tiles ausgefüllt wird und am Rand nichts übrig bleibt
    if (img->get_width() % max_tilewidth != 0)
    {
        this->tilecount_width++;
    }
    if (img->get_height() % max_tileheight != 0)
    {
        this->tilecount_height++;
    }

    PRINTLN("tc_width = " << tilecount_width << " tc_height=" << tilecount_height);

    //Jetzt hat man die benötigten informationen und man muss nur noch die tiles initialisieren
    for (long ih = 0; ih < tilecount_height; ih++)
    {
        for (long iw = 0; iw < tilecount_width; iw++)
        {
            boost::shared_ptr<tile> pt(new tile(img, grid_width, grid_height, iw, ih));
            this->vc_tile_array.push_back(pt);
        }
    }

}

void tiled_image::unwrap_tiles(boost::shared_ptr<abstract_tile_unwrapper> uw) {
    boost::shared_ptr<tile> cur_tile;
    
    //1) First unwrap all the tiles that are guaranteed to have the same size
    //   these are all the tiles except (possibly) for the rightmost(iw=W-1) column
    //   and the bottom row (ih=H-1)
    long W = this->tilecount_width;
    long H = this->tilecount_height;
    long iw, ih;
    for (ih = 0; ih < H - 1; ih++) {
        for (iw = 0; iw < W - 1; iw++) {
            cur_tile = this->get_tile_at(iw, ih);
            uw->unwrap(cur_tile);
            //            PRINTLN("Unwrapping tile at index (" << iw << "," << ih << ")" );
        }
    }

    //2) now unwrap right column EXCLUDING the bottom right tile (iw=W-1, and ih=H-1)
    iw = W - 1;
    for (int ih = 0; ih < H - 1; ih++) {
        cur_tile = this->get_tile_at(iw, ih);
        uw->unwrap(cur_tile);
        //       PRINTLN("Unwrapping tile at index (" << iw << "," << ih << ")" );
    }

    //3) Lastly unwrap bottom row INCLUDING the bottom right tile (iw=W-1, and ih=H-1)
    ih = H - 1;
    for (int iw = 0; iw < W; iw++) {
        cur_tile = this->get_tile_at(iw, ih);
        uw->unwrap(cur_tile);
        //        PRINTLN("Unwrapping tile at index (" << iw << "," << ih << ")" );
    }

}

//void tiled_image::apply_offsets() {
//   std::vector<boost::shared_ptr<tile> >::iterator it;
//   for (it = this->vc_tile_array.begin(); it != this->vc_tile_array.end(); ++it) {
//      (*it)->apply_value(); //Every pixel in every tile will get value = value + t.offset + tg.offset. t.offset is set to 0 afterwards
//   }
//}

/* Local function to test if all junctions are properly implemented */
void tiled_image::test_all_junctions_test() {
    boost::shared_ptr<tile> t;
    boost::shared_ptr<tile> tUp;
    boost::shared_ptr<tile> tDown;
    boost::shared_ptr<tile> tLeft;
    boost::shared_ptr<tile> tRight;

    for (int w = 1; w < this->tilecount_width - 1; w++) {
        for (int h = 1; h < this->tilecount_height - 1; h++) {
            t = this->get_tile_at(w, h);
            tUp = this->get_tile_at(w, h - 1);
            tDown = this->get_tile_at(w, h + 1);
            tLeft = this->get_tile_at(w - 1, h);
            tRight = this->get_tile_at(w + 1, h);
            //         juncUp = this->get_junction(tile::UP);
            //         juncDown = this->get_junction(tile::DOWN);
            //         juncLeft = this->get_junction(tile::LEFT);
            //         juncRight = this->get_junction(tile::RIGHT);
            if (this->get_junction(t, tile::UP)->calc_junction_mean_difference() != this->get_junction(tUp, tile::DOWN)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction up... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tile::DOWN)->calc_junction_mean_difference() != this->get_junction(tDown, tile::UP)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction down... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tile::LEFT)->calc_junction_mean_difference() != this->get_junction(tLeft, tile::RIGHT)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction down... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tile::RIGHT)->calc_junction_mean_difference() != this->get_junction(tRight, tile::LEFT)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction down... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tUp)->calc_junction_mean_difference() != this->get_junction(tUp, t)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction up... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tDown)->calc_junction_mean_difference() != this->get_junction(tDown, t)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction down... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tLeft)->calc_junction_mean_difference() != this->get_junction(tLeft, t)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction down... Failure in (w,h): " << w << " " << h);
            }
            if (this->get_junction(t, tRight)->calc_junction_mean_difference() != this->get_junction(tRight, t)->calc_junction_mean_difference()) {
                PRINTLN("Difference in junction down... Failure in (w,h): " << w << " " << h);
            }
        }
    }
    PRINTLN("No errors in junctions detected...");
}

void tiled_image::gradient(int dim, sharedptr<abstract_gradient_calculator> gcalc) //!todo exception
{
    if (!(dim == 0 || dim == 1)) {
        dim = 0; //todo exception
    }

    long W = this->tilecount_width;
    long H = this->tilecount_height;
    long iw, ih;
    sharedptr<tile> ptr_cur_tile;
    //no performance optimization. If needed, just make analogous to unwrap_tiles method.
    for (iw = 0; iw < W; iw++) {
        for (ih = 0; ih < H; ih++) {
            ptr_cur_tile = this->get_tile_at(iw, ih);
            (*ptr_cur_tile) = (gcalc->get_gradient_tile(ptr_cur_tile)->at(dim));
        }
    }

}
