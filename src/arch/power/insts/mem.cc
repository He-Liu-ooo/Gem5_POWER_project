/*
 * Copyright (c) 2009 The University of Edinburgh
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
 */

#include "arch/power/insts/mem.hh"

#include "base/loader/symtab.hh"

using namespace PowerISA;

std::string
MemOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    return csprintf("%-10s", mnemonic);
}


std::string
MemDispOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the destination only for a load
    if (!flags[IsStore]) {
        if (_numDestRegs > 0) {

            // If the instruction updates the source register with the
            // EA, then this source register is placed in position 0,
            // therefore we print the last destination register.
            printReg(ss, destRegIdx(_numDestRegs - 1));
        }
    }

    // Print the data register for a store
    else {
        if (_numSrcRegs > 0) {
            printReg(ss, srcRegIdx(0));
        }
    }

    // Print the displacement
    ss << ", " << disp;
    ss << "(";

    // Print the address register for a load
    if (!flags[IsStore]) {
        if (_numSrcRegs > 0) {
            printReg(ss, srcRegIdx(0));
        }

        // The address register is skipped if it is R0
        else {
            ss << "0";
        }
    }

    // Print the address register for a store
    else {
        if (_numSrcRegs > 1) {
            printReg(ss, srcRegIdx(1));
        }

        // The address register is skipped if it is R0
        else {
            ss << "0";
        }
    }

    ss << ")";

    return ss.str();
}


std::string
MemDispShiftOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the destination only for a load
    if (!flags[IsStore]) {
        if (_numDestRegs > 0) {

            // If the instruction updates the source register with the
            // EA, then this source register is placed in position 0,
            // therefore we print the last destination register.
            printReg(ss, destRegIdx(_numDestRegs - 1));
        }
    }

    // Print the data register for a store
    else {
        if (_numSrcRegs > 0) {
            printReg(ss, srcRegIdx(0));
        }
    }

    // Print the displacement
    ss << ", " << (disp << 2);
    ss << "(";

    // Print the address register for a load
    if (!flags[IsStore]) {
        if (_numSrcRegs > 0) {
            printReg(ss, srcRegIdx(0));
        }

        // The address register is skipped if it is R0
        else {
            ss << "0";
        }
    }

    // Print the address register for a store
    else {
        if (_numSrcRegs > 1) {
            printReg(ss, srcRegIdx(1));
        }

        // The address register is skipped if it is R0
        else {
            ss << "0";
        }
    }

    ss << ")";

    return ss.str();
}


std::string
MemIndexOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the destination only for a load
    if (!flags[IsStore]) {
        if (_numDestRegs > 0) {

            // If the instruction updates the source register with the
            // EA, then this source register is placed in position 0,
            // therefore we print the last destination register.
            printReg(ss, destRegIdx(_numDestRegs - 1));
        }
    }

    // Print the data register for a store
    else {
        if (_numSrcRegs > 0) {
            printReg(ss, srcRegIdx(0));
        }
    }

    ss << ", ";

    // Print the address registers for a load
    if (!flags[IsStore]) {
        if (_numSrcRegs > 1) {
            printReg(ss, srcRegIdx(0));
            ss << ", ";
            printReg(ss, srcRegIdx(1));
        }

        // The first address register is skipped if it is R0
        else if (_numSrcRegs > 0) {
            ss << "0, ";
            printReg(ss, srcRegIdx(0));
        }
    }

    // Print the address registers for a store
    else {
        if (_numSrcRegs > 2) {
            printReg(ss, srcRegIdx(1));
            ss << ", ";
            printReg(ss, srcRegIdx(2));
        }

        // The first address register is skipped if it is R0
        else if (_numSrcRegs > 1) {
            ss << "0, ";
            printReg(ss, srcRegIdx(1));
        }
    }

    return ss.str();
}
