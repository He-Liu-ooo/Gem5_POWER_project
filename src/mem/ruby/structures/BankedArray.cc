/*
 * Copyright (c) 2012 Advanced Micro Devices, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Brad Beckmann
 *
 */

#include "mem/ruby/structures/BankedArray.hh"

#include "base/intmath.hh"
#include "mem/ruby/system/RubySystem.hh"

BankedArray::BankedArray(unsigned int banks, Cycles accessLatency,
                         unsigned int startIndexBit, RubySystem *rs)
    : m_ruby_system(rs)
{
    this->banks = banks;
    this->accessLatency = accessLatency;
    this->startIndexBit = startIndexBit;

    if (banks != 0) {
        bankBits = floorLog2(banks);   // #TODO why is floor eg.2 bits for banks = 5
    }

    busyBanks.resize(banks);    // assign space
}

// ME: this function tells us whether the idx's corresponding bank can be accessed 
// ME: the idx is the cache line's set number
bool
BankedArray::tryAccess(int64_t idx)
{
    if (accessLatency == 0)    // which means this bank will never have a duration of being accessed
        return true;

    unsigned int bank = mapIndexToBank(idx);    // find which bank to access
    assert(bank < banks);

    if (busyBanks[bank].endAccess >= curTick()) {   // busyBank:bank 已经被访问了  the bank is being accessing now
                                                    // this aims to ensure avoiding bank conflict
            return false;
    }

    return true;
}

// ME: this function is marking the idx's corresponding bank as being accessed
void
BankedArray::reserve(int64_t idx)
{
    if (accessLatency == 0)
        return;

    unsigned int bank = mapIndexToBank(idx);
    assert(bank < banks);

    if (busyBanks[bank].endAccess >= curTick()) {  // if the same bank is being accessed
        if (busyBanks[bank].startAccess == curTick() &&   // the only probability is that this bank is already being reserved
             busyBanks[bank].idx == idx) {
            // this is the same reservation (can happen when
            // e.g., reserve the same resource for read and write)
            return; // OK
        } else {
            panic("BankedArray reservation error");
        }
    }

    // if the same bank finished being accessed, now we can access it again

    busyBanks[bank].idx = idx;
    busyBanks[bank].startAccess = curTick();
    busyBanks[bank].endAccess = curTick() +
        (accessLatency-1) * m_ruby_system->clockPeriod();
}

unsigned int
BankedArray::mapIndexToBank(int64_t idx)
{
    if (banks == 1) {    // if there's only one bank
        return 0;
    }
    return idx % banks;  // easiest mapping, map with the unit of cache set 
}
