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
 * File:   debug_time.h
 * Author: b.steltner
 *
 * Created on 2. Dezember 2014, 10:25
 */

#ifndef DEBUG_TIME_H
#define	DEBUG_TIME_H

#include <time.h>
#include <chrono>
#include "debug/debug_out.h"

class debug_time {
public:

    debug_time() {
        this->start = std::chrono::system_clock::now();
    }

    virtual ~debug_time() {
        this->end = std::chrono::system_clock::now();
        diff = this->end-this->start;
        PRINTLN("Measured time (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() );
    }

    double get_time() {
        diff = std::chrono::system_clock::now()-this->start;
        return diff.count();
    }

    double reset_time() {
        diff = std::chrono::system_clock::now()-this->start;
        this->start = std::chrono::system_clock::now();
        return diff.count();
        
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start, end; 
    std::chrono::duration<double> diff; 
};


#endif	/* DEBUG_TIME_H */

