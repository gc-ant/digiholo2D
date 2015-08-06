#include "analyse/measure_pixel_energy.h"
#include "image/row_major_float_image.h"
#include "debug/debug_time.h"

measure_pixel_energy::measure_pixel_energy() {
    //empty
}

measure_pixel_energy::~measure_pixel_energy() {
    //empty
}

float measure_pixel_energy::calc(sharedptr<row_major_float_image> image) {
    debug_time * time = new debug_time();

    long W = image->get_width();
    long H = image->get_height();

    float total = 0.f;

    for (int iw = 1; iw < W - 1; iw++) { //inner square
        for (int ih = 1; ih < H - 1; ih++) { //inner square
            total += std::pow(image->get_pixel(iw, ih) - image->get_pixel(iw + 1, ih), 2.f); //right
            total += std::pow(image->get_pixel(iw, ih) - image->get_pixel(iw, ih + 1), 2.f); //down
            total += std::pow(image->get_pixel(iw, ih) - image->get_pixel(iw - 1, ih + 1), 2.f); //down left 
            total += std::pow(image->get_pixel(iw, ih) - image->get_pixel(iw + 1, ih + 1), 2.f); //down right
        }
    }
    for (int iw = 1; iw < W - 1; iw++) { //top line
        total += std::pow(image->get_pixel(iw, 0) - image->get_pixel(iw + 1, 0), 2.f); //right
        total += std::pow(image->get_pixel(iw, 0) - image->get_pixel(iw - 1, 0 + 1), 2.f); //down left 
        total += std::pow(image->get_pixel(iw, 0) - image->get_pixel(iw + 1, 0 + 1), 2.f); //down right
    }
    for (int ih = 0; ih < H - 1; ih++) { //left line
        total += std::pow(image->get_pixel(0, ih) - image->get_pixel(0, ih + 1), 2.f); //down
        total += std::pow(image->get_pixel(0, ih) - image->get_pixel(0 + 1, ih + 1), 2.f); //down right
    }
    for (int iw = 0; iw < W - 1; iw++) { //bottom line
        total += std::pow(image->get_pixel(iw, H - 1) - image->get_pixel(iw + 1, H - 1), 2.f); //right
    }
    for (int ih = 0; ih < H - 1; ih++) { //right line
        total += std::pow(image->get_pixel(W - 1, ih) - image->get_pixel(W - 1, ih + 1), 2.f); //down
        total += std::pow(image->get_pixel(W - 1, ih) - image->get_pixel(W - 2, ih + 1), 2.f); //down left 
    }
    total /= (float) (W * H);
    DEBUG_PRINTLN("Runtime: ");
    delete time;
    return total;
}



