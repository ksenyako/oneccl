/*
 Copyright 2016-2019 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "common/env/env.hpp"
#include "common/utils/spinlock.hpp"
#include "common/utils/yield.hpp"

ccl_spinlock::ccl_spinlock()
{
    flag.clear();
}

void ccl_spinlock::lock()
{
    size_t spin_count = env_data.spin_count;
    while (flag.test_and_set(std::memory_order_acquire))
    {
        spin_count--;
        if (!spin_count)
        {
            ccl_yield(env_data.yield_type);
            spin_count = 1;
        }
    }
}

void ccl_spinlock::unlock()
{
    flag.clear(std::memory_order_release);
}
