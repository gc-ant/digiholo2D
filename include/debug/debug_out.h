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
 * File:   debug_out.h
 * Author: g.antonopoulos
 *
 * Created on 14. April 2014, 16:32
 */

#ifndef DEBUG_OUT_HEADER_H
#define	DEBUG_OUT_HEADER_H

#include <iostream>
#define PRINT(x)   std::cout<< x <<std::flush;
#define PRINTLN(x) std::cout<< x << std::endl <<std::flush;

#endif	/* DEBUG_OUT_HEADER_H */

