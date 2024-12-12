/*
Implementation of the Memory class
Contains the function defintions

Author: Dhananjay Vijayaraghavan
Date Created: 11 Novemember 2024
*/

#include "Memory.h" 

// Function definitions

/*
Function: request
Request to read/write memory at particular address
    rw: Pin for read/write. 0 for read and 1 for write
    address: 32 bit address
    pf: Pin to determine if request is from prefetch unit
*/
void Memory::request(bool rw, uint32_t address, bool pf){
    // Do nothing

    if(metrics){
        metrics->inc_n_access();
        if(rw) metrics->inc_n_writes();
        else metrics->inc_n_reads();
    }
}

/*
Function: setnxt
Set the pointer to the next level of the memory heirarchy
    nxt: pointer to the next level
*/
void Memory::setNext(Memory *nxt){
    next_level = nxt;
}

/*Function: setMetrics
Set the pointer to the object containing metrics
Implemented like this to support sub-classes of Metrics
    mtc: Pointer to the Metrics object
*/
void Memory::setMetrics(Metrics *mtc){
    metrics = mtc;
}
