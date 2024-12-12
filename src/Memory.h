/*
Header file for a basic memory class. 
Designed to be used as a base class for various kinds of memory,
like cache, main memory. 

Author: Dhananjay Vijayaraghavan
Date Created: 11 November 2024
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <inttypes.h>

#include "Metrics.h"

class Memory{

    protected:
        Metrics *metrics = NULL;
        Memory *next_level = NULL;

    public:
        virtual void request(bool rw, uint32_t address, bool pf);
        virtual void setNext(Memory *nxt);
        virtual void setMetrics(Metrics *mtc);

};

#endif