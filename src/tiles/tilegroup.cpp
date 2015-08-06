#include "tiles/tile.h"

tilegroup::tilegroup() {

}

tilegroup::~tilegroup() {
    this->fl_ptr_tiles.clear(); //clear is ok for forward list => fl has no capacity, therefore size = 0 is all memory is freed   
}

//O(n)! @todo: make O(1) by saving the number of elements. Make sure number of Elements cannot be changed any other way.
int tilegroup::size() {
    return std::distance(this->fl_ptr_tiles.begin(), this->fl_ptr_tiles.end());
}

void tilegroup::add_value(float value) {
    std::forward_list<boost::weak_ptr<tile> >::iterator it;

    for (it = this->fl_ptr_tiles.begin(); it != this->fl_ptr_tiles.end(); ++it) {
        it->lock()->add_value(+value);
    }
}

bool tilegroup::add_unique_tile(sharedptr<tile> t, sharedptr<tilegroup> g) {
    for (weakptr<tile> wk_tile : this->fl_ptr_tiles) {

        if (!wk_tile.expired() && wk_tile.lock().get() == t.get()) { //weak ptr is valid && forward list contains the tile t
            return false; //nothing to add, tile is in forward list already
        }
    } //tile t not found in the forward list, therefore adding it into it.

    if (t->has_tilegroup() == true) {
        t->ptr_tilegroup.reset();
    }
    t->ptr_tilegroup = g;
    g->fl_ptr_tiles.push_front(t);
    return true;
}

void add_tile_to_group(boost::shared_ptr<tile> t, boost::shared_ptr<tilegroup> g) {
    if (t->has_tilegroup() == true) {
        t->ptr_tilegroup.reset();
    }
    t->ptr_tilegroup = g;
    g->fl_ptr_tiles.push_front(t);
}

void merge_tilegroups(boost::shared_ptr<tilegroup> g1, boost::shared_ptr<tilegroup> g2) {
    if (g1 == g2) return;
    //Merge g1 into g2
    std::forward_list<boost::weak_ptr<tile> >::iterator it;

    for (it = g1->fl_ptr_tiles.begin(); it != g1->fl_ptr_tiles.end(); ++it) { //Apply offset to every tile in the group
        it->lock()->ptr_tilegroup = g2;
    }
    /* Merge (move!) g1 into g2 */
    g2->fl_ptr_tiles.merge(g1->fl_ptr_tiles);
}