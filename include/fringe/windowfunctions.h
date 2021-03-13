//
// Created by georgios on 12.03.21.
//

#ifndef DIGIHOLO2D_WINDOWFUNCTIONS_H
#define DIGIHOLO2D_WINDOWFUNCTIONS_H

float hann_window(float r);

float pow_hann_window3(float r);

//http://de.wikipedia.org/wiki/Fensterfunktion
//Muss aber so abgewandelt werden, dass sie das Argument 0-1 akzeptiert und sinnvoll
//verarbeitet.
//ACHTUNG: DIES IST NICHT WIRKLICH EIN BLACKMAN-NUTTAL WINDOW sondern
//dem 1-blackman-nuttal
float blackman_nuttal_window(float r);

#endif //DIGIHOLO2D_WINDOWFUNCTIONS_H
