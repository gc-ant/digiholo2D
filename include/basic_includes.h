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
 * File:   basic_includes.h
 * Author: b.steltner
 *
 * Created on 25. November 2014, 11:36
 */

#ifndef BASIC_INCLUDES_H
#define	BASIC_INCLUDES_H

/**
 * This file contains the basic includes necessary for the majority of the files to 
 * shorten the include list of each file & reducing the amount of included headers 
 * which are excluded by the include-guard again. 
 * 
 * Furthermore it holds the most commoncly used typedefs (if existing) and templates.
 */

#include <boost/smart_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/math/constants/constants.hpp>
#include "debug/debug_out.h"

#define M_PI boost::math::constants::pi<float>()


/* C++03 standard */
//template <typename T> 
//struct shared   {
//    typedef boost::shared_ptr<T> ptr; 
//};
// Invoke w/: shared::ptr<smart_tile> 


/* C++11 standard */
template <typename T> using sharedptr = boost::shared_ptr<T>; 
// Invoke w/: sharedptr<float> sp(5.78);
//
template <typename T> using weakptr = boost::weak_ptr<T>; 
// Invoke w/: weakptr<float> sp(5.78);

template <typename T> using sharedarray = boost::shared_array<T>;






#endif	/* BASIC_INCLUDES_H */

