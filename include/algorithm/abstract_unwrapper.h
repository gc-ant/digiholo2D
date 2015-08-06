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
 * File:   abstract_unwrapper.h
 * Author: g.antonopoulos
 *
 * Created on 28. Oktober 2014, 08:32
 */

#ifndef ABSTRACT_UNWRAPPER_H
#define	ABSTRACT_UNWRAPPER_H

#include "basic_includes.h"
#include "image/float_image.h"

/**
 * This pure virtual class provides an interface for a general unwrapper that operates with
 * boost smart pointers.
 */
class abstract_unwrapper {
public:

    abstract_unwrapper() {
    }

    virtual ~abstract_unwrapper() {
    }

    /**
     * An asbtract method for phase unwrapping.
     * @param wrapped_phase_image This image contains the wrapped phase data.
     * @return A float_image with the unwrapped phase data. The image will
     * actually be a row_major_float_image.
     */
    virtual boost::shared_ptr<float_image> unwrap(boost::shared_ptr<float_image> wrapped_phase_image) = 0;
};


#endif	/* ABSTRACT_UNWRAPPER_H */

