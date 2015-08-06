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
 * File:   test_memory_leak.h
 * Author: b.steltner
 *
 * Created on 4. Dezember 2014, 11:03
 */

#ifndef TEST_MEMORY_LEAK_H
#define	TEST_MEMORY_LEAK_H

#include <vector>
#include "basic_includes.h"

//FORWARD DECLARATION START
class test_memory_leak_a;
class test_memory_leak_b;
//FORWARD DECLARATION END

class test_memory_leak {
public:
    test_memory_leak();
    //     ~test_memory_leak();
private:
    std::vector<long long> vc_long;
    std::vector<sharedptr<test_memory_leak_a> > vc_leak_a;
    std::vector<sharedptr<test_memory_leak_b> > vc_leak_b;
};

#endif	/* TEST_MEMORY_LEAK_H */

