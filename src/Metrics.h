/*
Header file for the Metrics class. The class implements 
the counters and the analysis of the performance.

Author: Dhananjay Vijayaraghavan
Date Created: 08 October 2024
*/

#ifndef METRICS_H
#define METRICS_H

#include <inttypes.h>

class Metrics{
    private:
        uint32_t n_access;
        uint32_t n_reads;
        uint32_t n_writes;
        uint32_t n_writebacks;
        uint32_t n_prefetches;
        uint32_t n_read_miss;
        uint32_t n_write_miss;
        uint32_t n_pf_read;
        uint32_t n_pf_read_miss;

    public:
    
        void inc_n_access();
        void inc_n_reads();
        void inc_n_writes();
        void inc_n_writebacks();
        void inc_n_prefetches();
        void inc_n_read_miss();
        void inc_n_write_miss();
        void inc_n_pf_read();
        void inc_n_pf_read_miss();

        uint32_t get_n_access();
        uint32_t get_n_reads();
        uint32_t get_n_writes();
        uint32_t get_n_writebacks();
        uint32_t get_n_prefetches();
        uint32_t get_n_read_miss();
        uint32_t get_n_write_miss();
        uint32_t get_n_pf_read();
        uint32_t get_n_pf_read_miss();

};

#endif