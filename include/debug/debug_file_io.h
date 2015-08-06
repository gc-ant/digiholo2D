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
 * File:   debug_file_io.h
 * Author: b.steltner
 *
 * Created on 2. Dezember 2014, 14:14
 */

#ifndef DEBUG_FILE_IO_H
#define	DEBUG_FILE_IO_H

#include <iostream>
#include <fstream>


class debug_file_io     {
public:
    //std constructor
    debug_file_io(std::string filename);
    
    
    
    virtual ~debug_file_io(); 
    
    void append_to_file(std::string parameter); 
    void clear_file(); 
    void read_file(); 
    
private:
    //!Hidden copy constructor
    debug_file_io(const debug_file_io&); 
    //!Hidden = operator, making the class non-copyable
    debug_file_io& operator=(const debug_file_io&);
    
    debug_file_io();
    std::ofstream myfile; 
};


#endif	/* DEBUG_FILE_IO_H */

