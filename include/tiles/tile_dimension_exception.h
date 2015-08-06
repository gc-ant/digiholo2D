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
 * File:   tile_dimension_exception.h
 * Author: g.antonopoulos
 *
 * Created on 2. Januar 2015, 09:09
 */

#ifndef TILE_DIMENSION_EXCEPTION_H
#define	TILE_DIMENSION_EXCEPTION_H

#include <boost/exception/all.hpp>
#include <exception>

class tile_dimension_exception : virtual boost::exception, virtual std::exception
{
public:
    tile_dimension_exception();
    tile_dimension_exception(const tile_dimension_exception& orig);
    virtual ~tile_dimension_exception();
private:
};

#endif	/* TILE_DIMENSION_EXCEPTION_H */

