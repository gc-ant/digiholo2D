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
 * File:   test_memory_leak_a.h
 * Author: b.steltner
 *
 * Created on 4. Dezember 2014, 11:13
 */

#ifndef TEST_MEMORY_LEAK_A_H
#define	TEST_MEMORY_LEAK_A_H

#include "basic_includes.h"

class test_memory_leak_a    {
public:
    test_memory_leak_a(long long variable)    {
        this->variable = variable; 
    }
//    ~test_memory_leak_a();
    
private:
    test_memory_leak_a();
    long long variable; 
};

#endif	/* TEST_MEMORY_LEAK_A_H */

