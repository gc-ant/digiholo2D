/* 
 * File:   test_memory_leak.cpp
 * Author: b.steltner
 *
 * Created on 4. Dezember 2014, 11:03
 */

#include <algorithm>

#include "debug/test_memory_leak.h"

#include "debug/test_memory_leak_a.h"
#include "debug/test_memory_leak_b.h"
#include "debug/debug_out.h"

test_memory_leak::test_memory_leak() {
   const int number = 10000000;
   this->vc_long.reserve(number);
   this->vc_leak_a.reserve(number);
   this->vc_leak_b.reserve(number);

   for (int i = 0; i < number; i++) {
      this->vc_long.push_back(i);
      sharedptr<test_memory_leak_a> temp_a(new test_memory_leak_a((long long) i));
      this->vc_leak_a.push_back(temp_a);
      sharedptr<test_memory_leak_b> temp_b(new test_memory_leak_b((long long) i));
      this->vc_leak_b.push_back(temp_b);
      //      this->vc_leak_a.push_back(new test_memory_leak_a((long long)i));
   }
      PRINTLN("Done");
}

