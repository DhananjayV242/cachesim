/* 
Implementation of the Metrics class
Contains the function definitions to get and increment 
metrics related to cache performance

Author: Dhananjay Vijayaraghavan
Date Created: 11 November 2024
*/

#include "Metrics.h"

/* Functions to increment metrics */

void Metrics::inc_n_access(){
    ++n_access;
}

void Metrics::inc_n_reads(){
    ++n_reads;
}

void Metrics::inc_n_writes(){
    ++n_writes;
}

void Metrics::inc_n_writebacks(){
    ++n_writebacks;
}

void Metrics::inc_n_prefetches(){
    ++n_prefetches;
}

void Metrics::inc_n_read_miss(){
    ++n_read_miss;
}

void Metrics::inc_n_write_miss(){
    ++n_write_miss;
}

void Metrics::inc_n_pf_read(){
    ++n_pf_read;
}

void Metrics::inc_n_pf_read_miss(){
    ++n_pf_read_miss;
}

/* Functions to get value of metric */

uint32_t Metrics::get_n_access(){
    return n_access;
}

uint32_t Metrics::get_n_reads(){
    return n_reads;
}

uint32_t Metrics::get_n_writes(){
    return n_writes;
}

uint32_t Metrics::get_n_writebacks(){
    return n_writebacks;
}

uint32_t Metrics::get_n_prefetches(){
    return n_prefetches;
}

uint32_t Metrics::get_n_read_miss(){
    return n_read_miss;
}

uint32_t Metrics::get_n_write_miss(){
    return n_write_miss;
}

uint32_t Metrics::get_n_pf_read(){
    return n_pf_read;
}

uint32_t Metrics::get_n_pf_read_miss(){
    return n_pf_read_miss;
}