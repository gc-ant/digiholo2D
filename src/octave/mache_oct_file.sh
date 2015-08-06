#!/bin/bash

echo "*** Mache das zu OCT File ***"
echo "*** 32bit MINGW muss im PATH sein!!***"

mkoctfile -I'../../include' -I'../../include/block_srncp ' -output oct_tilebased_unwrap.oct oct_tilebased_unwrap.cpp /D/G_Antonopoulos/Code/digiholo2D/src/*.cpp